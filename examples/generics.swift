// Swift Generics Examples

// Generic function
func swapValues<T>(_ a: inout T, _ b: inout T) {
    let temp = a
    a = b
    b = temp
}

// Generic collection
struct Stack<Element> {
    private var items: [Element] = []
    
    mutating func push(_ item: Element) {
        items.append(item)
    }
    
    mutating func pop() -> Element? {
        return items.isEmpty ? nil : items.removeLast()
    }
    
    var count: Int {
        return items.count
    }
    
    var isEmpty: Bool {
        return items.isEmpty
    }
}

// Generic protocol with associated type
protocol Container {
    associatedtype Item
    mutating func add(_ item: Item)
    var count: Int { get }
}

// Class implementing generic protocol
class Box<T>: Container {
    var items = [T]()
    
    func add(_ item: T) {
        items.append(item)
    }
    
    var count: Int {
        return items.count
    }
}

// Generic constraints
func findIndex<T: Equatable>(of valueToFind: T, in array: [T]) -> Int? {
    for (index, value) in array.enumerated() {
        if value == valueToFind {
            return index
        }
    }
    return nil
}

// Generic where clause
extension Stack where Element: Equatable {
    func isTop(_ item: Element) -> Bool {
        guard let topItem = items.last else {
            return false
        }
        return topItem == item
    }
}