// Swift operators and expressions


// Basic arithmetic operators
let sum = 5 + 3
let difference = 10 - 2
let product = 4 * 6
let quotient = 12 / 3
let remainder = 10 % 3

// Compound assignment operators
var number = 5
number += 3
number -= 2
number *= 4
number /= 2

// Comparison operators
let isEqual = 5 == 5
let isNotEqual = 5 != 6
let isGreater = 7 > 3
let isLess = 2 < 8
let isGreaterOrEqual = 10 >= 10
let isLessOrEqual = 5 <= 6

// Logical operators
let logicalAND = true && false
let logicalOR = true || false
let logicalNOT = !true

// Range operators
let closedRange = 1...5
let halfOpenRange = 1..<5

// Ternary conditional operator
let ternary = (5 > 3) ? "Yes" : "No"

// Nil-coalescing operator
let defaultName = "Guest"
let inputName: String? = nil
let userName = inputName ?? defaultName

// Optional chaining
let optionalString: String? = "Hello"
let uppercaseString = optionalString?.uppercased()

// Bitwise operators
let bitwiseAND = 0b1010 & 0b1100
let bitwiseOR = 0b1010 | 0b1100
let bitwiseXOR = 0b1010 ^ 0b1100
let bitwiseNOT = ~0b1010
let leftShift = 1 << 2
let rightShift = 8 >> 2

// Custom operators
prefix operator +++
prefix func +++ (value: inout Int) -> Int {
    value += 1
    return value
}
