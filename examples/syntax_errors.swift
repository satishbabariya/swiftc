// Swift file with deliberate syntax errors for testing

// Unterminated string literal
let message = "Hello, world

// Missing closing brace
func calculateArea(width: Int, height: Int) -> Int {
    let area = width * height
    return area

// Invalid identifier with special characters
let @invalid-name = 42

// Invalid character in numeric literal
let number = 123$456

// Unterminated comment
/* This is a multi-line comment
   that is not properly terminated

// Unexpected token
let result = 10 + * 20

// Mismatched delimiters
let array = [1, 2, 3)

// Multiple errors on one line
let x = "unterminated string with invalid \u characters

// Unterminated string interpolation
let name = "Swift"
let greeting = "Hello, \(name"