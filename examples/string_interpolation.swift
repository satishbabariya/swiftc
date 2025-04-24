// Swift String Interpolation Examples

// Basic string interpolation
let name = "Swift"
let version = 5.7
let greeting = "Hello, \(name) \(version)!"

// String interpolation with expressions
let a = 10
let b = 20
let expressionResult = "The sum of \(a) and \(b) is \(a + b)"

// String interpolation with function calls
func double(_ number: Int) -> Int {
    return number * 2
}
let functionResult = "Double of \(a) is \(double(a))"

// Custom string interpolation
struct Person {
    let name: String
    let age: Int
}

extension String.StringInterpolation {
    mutating func appendInterpolation(person: Person) {
        appendInterpolation("Name: \(person.name), Age: \(person.age)")
    }
    
    mutating func appendInterpolation(formatted number: Int) {
        let formatter = NumberFormatter()
        formatter.numberStyle = .spellOut
        if let result = formatter.string(from: NSNumber(value: number)) {
            appendLiteral(result)
        } else {
            appendLiteral("\(number)")
        }
    }
    
    mutating func appendInterpolation(_ value: Double, precision: Int) {
        let format = "%.\(precision)f"
        let formattedString = String(format: format, value)
        appendLiteral(formattedString)
    }
}

// Using custom string interpolation
let person = Person(name: "John", age: 30)
let personDescription = "Person details: \(person: person)"
let spelledOut = "The number is \(formatted: 42)"
let pi = "Pi is approximately \(3.14159, precision: 2)"

// Multiline string with interpolation
let multiline = """
This is a multiline string.
It contains multiple lines of text.
The current Swift version is \(version).
"""