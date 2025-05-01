// Swift Protocols and Extensions Examples

// Basic protocol definition
protocol Identifiable {
    var id: String { get }
    func identify()
}

// Protocol implementation
struct User: Identifiable {
    var id: String
    var name: String
    
    func identify() {
        print("User ID: \(id), name: \(name)")
    }
}

// Protocol with associated types
protocol DataProvider {
    associatedtype DataType
    func getData() -> DataType
}

// Protocol inheritance
protocol Animal {
    var species: String { get }
    func makeSound()
}

protocol Pet: Animal {
    var name: String { get }
    var owner: String { get }
}

// Protocol composition
protocol Named {
    var name: String { get }
}

protocol Aged {
    var age: Int { get }
}

func wishHappyBirthday(to celebrator: Named & Aged) {
    print("Happy birthday, \(celebrator.name)! You're \(celebrator.age) years old!")
}

// Basic extension
extension String {
    var isPalindrome: Bool {
        let chars = Array(self.lowercased())
        return chars == chars.reversed()
    }
    
    func repeatThreeTimes() -> String {
        return String(repeating: self, count: 3)
    }
}

// Extension with protocol conformance
extension Double: Identifiable {
    var id: String {
        return String(self)
    }
    
    func identify() {
        print("This is the number: \(self)")
    }
}

// Protocol extension
extension Collection {
    func summarize() -> String {
        return "Collection contains \(count) elements"
    }
}

// Protocol extension with constraints
extension Collection where Element: Numeric {
    func sum() -> Element {
        return reduce(0, +)
    }
}