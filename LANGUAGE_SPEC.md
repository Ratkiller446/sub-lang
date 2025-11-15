# SUB Language Specification v1.0

## 1. Introduction

SUB (Simple Universal Builder) is a cross-platform programming language designed for ease of learning and maximum portability. It uses a blockchain-inspired syntax with hash symbols (#) to create visually clear, chained code blocks.

## 2. Syntax

### 2.1 Comments
```sub
// Single-line comment
```

### 2.2 Variables
Variables are declared using the `#var` keyword:
```sub
#var name = "John"
#var age = 25
#var price = 19.99
#var isActive = true
```

### 2.3 Data Types
- **String**: Text enclosed in quotes ("" or '')
- **Number**: Integer or floating-point
- **Boolean**: true or false
- **Array**: Ordered collection [1, 2, 3]
- **Object**: Key-value pairs {key: value}

### 2.4 Functions
```sub
#function functionName(param1, param2)
    // function body
    #return result
#end
```

### 2.5 Conditionals
```sub
#if condition
    // code
#elif otherCondition
    // code
#else
    // code
#end
```

### 2.6 Loops

**For Loop:**
```sub
#for i in range(10)
    #print(i)
#end
```

**While Loop:**
```sub
#while condition
    // code
#end
```

### 2.7 UI Components
```sub
#ui.window(title="App", width=800, height=600)
    #ui.button(text="Click", onclick=handler)
    #ui.label(text="Hello")
    #ui.input(placeholder="Name", id="input1")
#end
```

### 2.8 Embedded Languages
```sub
#embed python
    # Python code here
#endembed

#embed javascript
    // JavaScript code here
#endembed
```

## 3. Blockchain Method

The hash symbol (#) serves as a "blockchain" operator that:
- Marks the start of each statement
- Creates visual chaining of operations
- Improves code readability
- Enforces structured programming

## 4. Cross-Platform Compilation

SUB compiles to native code for multiple platforms:

| Platform | Output | Framework |
|----------|--------|-----------|  
| Android | Java/Kotlin | Android SDK |
| iOS | Swift | UIKit |
| Web | JavaScript | HTML5 |
| Windows | C/C++ | Win32 |
| macOS | Swift/ObjC | Cocoa |
| Linux | C | GTK |

## 5. Standard Library

### 5.1 I/O
- `print(value)` - Output to console
- `input(prompt)` - Get user input

### 5.2 String Operations
- `length(str)` - String length
- `concat(str1, str2)` - Concatenate
- `substring(str, start, end)` - Extract substring

### 5.3 Math Operations
- `abs(x)` - Absolute value
- `sqrt(x)` - Square root
- `pow(x, y)` - Power
- `random()` - Random number

### 5.4 UI Operations
- `ui.alert(message)` - Show alert
- `ui.getInput(id)` - Get input value
- `ui.setProperty(id, prop, value)` - Set property

## 6. File Extension

SUB source files use the `.sb` extension.

## 7. Compiler Phases

1. **Lexical Analysis** - Tokenization
2. **Syntax Analysis** - Parsing
3. **Semantic Analysis** - Type checking
4. **Optimization** - Code optimization
5. **Code Generation** - Platform-specific output

## 8. Reserved Keywords

var, function, if, elif, else, for, while, return, end, embed, endembed, ui, import, from, try, catch, class, extends, new, this, true, false, null

## 9. Future Features

- Object-oriented programming
- Asynchronous operations
- Package manager
- Debugger integration
- IDE support

---
**Version**: 1.0  
**Last Updated**: 2025  
**Status**: Active Development
