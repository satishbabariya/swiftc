// This is a basic Swift example file with common syntax

// Variables and constants
let pi = 3.14159
var count = 42
var name = "Swift"

// Control flow
if count > 40 {
    print("Count is greater than 40")
} else {
    print("Count is 40 or less")
}

// Functions
func greet(person: String) -> String {
    return "Hello, \(person)!"
}


// Calling a function
let greeting = greet(person: "World")

// Arrays
var fruits = ["Apple", "Banana", "Orange"]
fruits.append("Mango")

// Dictionaries
var scores = ["Alice": 95, "Bob": 87, "Charlie": 92]
scores["Dave"] = 88

// Class definition
class Person {
    var name: String
    var age: Int
    
    init(name: String, age: Int) {
        self.name = name
        self.age = age
    }
    
    func describe() -> String {
        return "\(name) is \(age) years old"
    }
}

// Creating an instance
let john = Person(name: "John", age: 30)
