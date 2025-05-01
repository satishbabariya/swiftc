// Advanced Swift types and features


// Enumerations
enum Direction {
    case north
    case south
    case east
    case west
}

// Enum with associated values
enum Barcode {
    case upc(Int, Int, Int, Int)
    case qrCode(String)
}

// Using enum values
let productBarcode = Barcode.upc(8, 85909, 51226, 3)

// Structs
struct Point {
    var x: Double
    var y: Double
}

// Value types
var point1 = Point(x: 0.0, y: 0.0)
let point2 = point1
point1.x = 1.0

// Optionals
var optionalName: String? = "John"
if let name = optionalName {
    print("Hello, \(name)")
}

// Protocols
protocol Vehicle {
    var numberOfWheels: Int { get }
    func start()
}

// Extensions
extension Int {
    func squared() -> Int {
        return self * self
    }
}

// Generic functions
func swapValues<T>(_ a: inout T, _ b: inout T) {
    let temp = a
    a = b
    b = temp
}

// Type aliases
typealias AudioSample = UInt16
