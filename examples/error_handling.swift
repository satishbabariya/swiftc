// Swift error handling examples

// Define an error type
enum VendingMachineError: Error {
    case invalidSelection
    case insufficientFunds(coinsNeeded: Int)
    case outOfStock
}

// Function that throws errors
func vend(itemNamed name: String, coinsDeposited: Int) throws -> String {
    let itemPrice = 10
    
    guard name == "Candy" else {
        throw VendingMachineError.invalidSelection
    }
    
    guard coinsDeposited >= itemPrice else {
        throw VendingMachineError.insufficientFunds(coinsNeeded: itemPrice - coinsDeposited)
    }
    
    // Item is in stock, so vend it
    return "Dispensing \(name)"
}

// Using do-catch
do {
    let item = try vend(itemNamed: "Candy", coinsDeposited: 8)
    print(item)
} catch VendingMachineError.invalidSelection {
    print("Invalid selection")
} catch VendingMachineError.insufficientFunds(let coinsNeeded) {
    print("Insufficient funds. Please insert \(coinsNeeded) more coins.")
} catch VendingMachineError.outOfStock {
    print("Out of stock.")
} catch {
    print("Unexpected error: \(error).")
}

// Try? and try!
let result = try? vend(itemNamed: "Candy", coinsDeposited: 15)

// Function with throws in signature that calls another throwing function
func buyFavoriteSnack(person: String, vendingMachine: String) throws -> String {
    return try vend(itemNamed: "Candy", coinsDeposited: 20)
}

// Defer statement
func processFile(filename: String) throws {
    // Open file
    let file = "Some file handle"
    
    defer {
        // This will be executed at the end of the scope
        print("Closing file")
    }
    
    // Process the file
    print("Processing \(filename)")
    
    // File will be closed when the function ends
}
