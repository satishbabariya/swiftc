// Swift Property Wrappers Examples

// Basic property wrapper
@propertyWrapper
struct TrimmedString {
    private var value: String = ""
    
    var wrappedValue: String {
        get { return value }
        set { value = newValue.trimmingCharacters(in: .whitespacesAndNewlines) }
    }
    
    init(wrappedValue initialValue: String) {
        self.wrappedValue = initialValue
    }
}

// Using the property wrapper
struct User {
    @TrimmedString var username: String
    @TrimmedString var email: String
}

// Property wrapper with a projected value
@propertyWrapper
struct Capitalized {
    private var value: String = ""
    
    var wrappedValue: String {
        get { return value }
        set { value = newValue.capitalized }
    }
    
    var projectedValue: String {
        return value.uppercased()
    }
    
    init(wrappedValue initialValue: String) {
        self.wrappedValue = initialValue
    }
}

// Using projected value of property wrapper
struct Book {
    @Capitalized var title: String
    
    func printDetails() {
        print("Title: \(title)")
        print("UPPERCASE: \($title)")
    }
}

// Property wrapper with arguments
@propertyWrapper
struct Clamped<T: Comparable> {
    private var value: T
    private let range: ClosedRange<T>
    
    var wrappedValue: T {
        get { return value }
        set { value = min(max(range.lowerBound, newValue), range.upperBound) }
    }
    
    init(wrappedValue: T, range: ClosedRange<T>) {
        self.range = range
        self.value = min(max(range.lowerBound, wrappedValue), range.upperBound)
    }
}

// Using property wrapper with arguments
struct Temperature {
    @Clamped(range: -273.15...100.0) var celsius: Double
}

// Multiple property wrappers
struct Profile {
    @TrimmedString @Capitalized var name: String
}