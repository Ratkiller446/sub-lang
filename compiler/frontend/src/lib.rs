//! SUB Language Frontend - Rust Implementation
//! High-performance lexer and parser with FFI exports

use rayon::prelude::*;
use std::ffi::{CStr, CString};
use std::os::raw::c_char;
use thiserror::Error;

#[derive(Debug, Clone, Copy, PartialEq)]
#[repr(C)]
pub enum TokenType {
    Hash, Var, Const, Function, If, Elif, Else,
    For, While, Do, Return, End, Break, Continue,
    Try, Catch, Finally, Throw,
    Embed, Endembed, Cpp, C, Python, Javascript, Rust,
    Ui, Class, Extends, New, This,
    Identifier, Number, StringLit, True, False, Null,
    Operator, Arrow, LParen, RParen, LBrace, RBrace,
    Dot, Comma, Colon, Semicolon, Newline, Eof,
}

#[repr(C)]
pub struct Token {
    pub token_type: TokenType,
    pub value: *mut c_char,
    pub line: i32,
    pub column: i32,
}

#[repr(C)]
pub struct TokenArray {
    pub tokens: *mut Token,
    pub count: usize,
}

#[derive(Error, Debug)]
pub enum LexerError {
    #[error("Unexpected character '{0}' at line {1}, column {2}")]
    UnexpectedChar(char, usize, usize),
    #[error("Unterminated string at line {0}")]
    UnterminatedString(usize),
}

pub struct Lexer {
    source: Vec<char>,
    position: usize,
    line: usize,
    column: usize,
}

impl Lexer {
    pub fn new(source: &str) -> Self {
        Lexer {
            source: source.chars().collect(),
            position: 0,
            line: 1,
            column: 1,
        }
    }

    fn current(&self) -> Option<char> {
        self.source.get(self.position).copied()
    }

    fn peek(&self, offset: usize) -> Option<char> {
        self.source.get(self.position + offset).copied()
    }

    fn advance(&mut self) -> Option<char> {
        let ch = self.current()?;
        self.position += 1;
        if ch == '\n' {
            self.line += 1;
            self.column = 1;
        } else {
            self.column += 1;
        }
        Some(ch)
    }

    fn skip_whitespace(&mut self) {
        while let Some(ch) = self.current() {
            if ch == ' ' || ch == '\t' || ch == '\r' {
                self.advance();
            } else {
                break;
            }
        }
    }

    fn skip_comment(&mut self) {
        if self.current() == Some('/') && self.peek(1) == Some('/') {
            while self.current().is_some() && self.current() != Some('\n') {
                self.advance();
            }
        }
    }

    fn read_string(&mut self, quote: char) -> Result<String, LexerError> {
        let start_line = self.line;
        self.advance(); // Skip opening quote
        let mut value = String::new();

        while let Some(ch) = self.current() {
            if ch == quote {
                self.advance();
                return Ok(value);
            }
            if ch == '\\' {
                self.advance();
                if let Some(escaped) = self.advance() {
                    value.push(match escaped {
                        'n' => '\n',
                        't' => '\t',
                        'r' => '\r',
                        '\\' => '\\',
                        '"' => '"',
                        '\'' => '\'',
                        _ => escaped,
                    });
                }
            } else {
                value.push(ch);
                self.advance();
            }
        }
        Err(LexerError::UnterminatedString(start_line))
    }

    fn read_number(&mut self) -> String {
        let mut value = String::new();
        while let Some(ch) = self.current() {
            if ch.is_ascii_digit() || ch == '.' || ch == '_' {
                if ch != '_' {
                    value.push(ch);
                }
                self.advance();
            } else {
                break;
            }
        }
        value
    }

    fn read_identifier(&mut self) -> String {
        let mut value = String::new();
        while let Some(ch) = self.current() {
            if ch.is_alphanumeric() || ch == '_' {
                value.push(ch);
                self.advance();
            } else {
                break;
            }
        }
        value
    }

    fn keyword_type(&self, word: &str) -> TokenType {
        match word {
            "var" => TokenType::Var,
            "const" => TokenType::Const,
            "function" => TokenType::Function,
            "if" => TokenType::If,
            "elif" => TokenType::Elif,
            "else" => TokenType::Else,
            "for" => TokenType::For,
            "while" => TokenType::While,
            "do" => TokenType::Do,
            "return" => TokenType::Return,
            "end" => TokenType::End,
            "break" => TokenType::Break,
            "continue" => TokenType::Continue,
            "try" => TokenType::Try,
            "catch" => TokenType::Catch,
            "finally" => TokenType::Finally,
            "throw" => TokenType::Throw,
            "embed" => TokenType::Embed,
            "endembed" => TokenType::Endembed,
            "cpp" => TokenType::Cpp,
            "c" => TokenType::C,
            "python" => TokenType::Python,
            "javascript" => TokenType::Javascript,
            "rust" => TokenType::Rust,
            "ui" => TokenType::Ui,
            "class" => TokenType::Class,
            "extends" => TokenType::Extends,
            "new" => TokenType::New,
            "this" => TokenType::This,
            "true" => TokenType::True,
            "false" => TokenType::False,
            "null" => TokenType::Null,
            _ => TokenType::Identifier,
        }
    }

    pub fn tokenize(&mut self) -> Result<Vec<(TokenType, String, usize, usize)>, LexerError> {
        let mut tokens = Vec::new();

        while let Some(ch) = self.current() {
            self.skip_whitespace();
            self.skip_comment();

            if self.current().is_none() {
                break;
            }

            let ch = self.current().unwrap();
            let line = self.line;
            let column = self.column;

            match ch {
                '\n' => {
                    tokens.push((TokenType::Newline, String::new(), line, column));
                    self.advance();
                }
                '#' => {
                    tokens.push((TokenType::Hash, "#".to_string(), line, column));
                    self.advance();
                }
                '(' => {
                    tokens.push((TokenType::LParen, "(".to_string(), line, column));
                    self.advance();
                }
                ')' => {
                    tokens.push((TokenType::RParen, ")".to_string(), line, column));
                    self.advance();
                }
                '{' => {
                    tokens.push((TokenType::LBrace, "{".to_string(), line, column));
                    self.advance();
                }
                '}' => {
                    tokens.push((TokenType::RBrace, "}".to_string(), line, column));
                    self.advance();
                }
                '.' => {
                    tokens.push((TokenType::Dot, ".".to_string(), line, column));
                    self.advance();
                }
                ',' => {
                    tokens.push((TokenType::Comma, ",".to_string(), line, column));
                    self.advance();
                }
                ':' => {
                    tokens.push((TokenType::Colon, ":".to_string(), line, column));
                    self.advance();
                }
                ';' => {
                    tokens.push((TokenType::Semicolon, ";".to_string(), line, column));
                    self.advance();
                }
                '=' if self.peek(1) == Some('>') => {
                    tokens.push((TokenType::Arrow, "=>".to_string(), line, column));
                    self.advance();
                    self.advance();
                }
                '"' | '\'' => {
                    let value = self.read_string(ch)?;
                    tokens.push((TokenType::StringLit, value, line, column));
                }
                _ if ch.is_ascii_digit() => {
                    let value = self.read_number();
                    tokens.push((TokenType::Number, value, line, column));
                }
                _ if ch.is_alphabetic() || ch == '_' => {
                    let value = self.read_identifier();
                    let token_type = self.keyword_type(&value);
                    tokens.push((token_type, value, line, column));
                }
                '+' | '-' | '*' | '/' | '=' | '<' | '>' | '!' | '&' | '|' => {
                    let mut op = ch.to_string();
                    self.advance();
                    if let Some(next) = self.current() {
                        if next == '=' || (ch == next && (ch == '&' || ch == '|')) {
                            op.push(next);
                            self.advance();
                        }
                    }
                    tokens.push((TokenType::Operator, op, line, column));
                }
                _ => {
                    return Err(LexerError::UnexpectedChar(ch, line, column));
                }
            }
        }

        tokens.push((TokenType::Eof, String::new(), self.line, self.column));
        Ok(tokens)
    }
}

#[no_mangle]
pub extern "C" fn sub_frontend_tokenize(source: *const c_char) -> *mut TokenArray {
    if source.is_null() {
        return std::ptr::null_mut();
    }

    let c_str = unsafe { CStr::from_ptr(source) };
    let source_str = match c_str.to_str() {
        Ok(s) => s,
        Err(_) => return std::ptr::null_mut(),
    };

    let mut lexer = Lexer::new(source_str);
    let tokens = match lexer.tokenize() {
        Ok(t) => t,
        Err(_) => return std::ptr::null_mut(),
    };

    let c_tokens: Vec<Token> = tokens
        .into_iter()
        .map(|(token_type, value, line, column)| {
            let c_value = if value.is_empty() {
                std::ptr::null_mut()
            } else {
                CString::new(value).unwrap().into_raw()
            };
            Token {
                token_type,
                value: c_value,
                line: line as i32,
                column: column as i32,
            }
        })
        .collect();

    let count = c_tokens.len();
    let tokens_ptr = Box::into_raw(c_tokens.into_boxed_slice()) as *mut Token;

    Box::into_raw(Box::new(TokenArray {
        tokens: tokens_ptr,
        count,
    }))
}

#[no_mangle]
pub extern "C" fn sub_frontend_free_tokens(token_array: *mut TokenArray) {
    if token_array.is_null() {
        return;
    }

    unsafe {
        let array = Box::from_raw(token_array);
        let tokens = Vec::from_raw_parts(array.tokens, array.count, array.count);
        for token in tokens {
            if !token.value.is_null() {
                let _ = CString::from_raw(token.value);
            }
        }
    }
}
