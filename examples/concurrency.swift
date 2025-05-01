// Swift concurrency examples


// Asynchronous function
func fetchUserID(from server: String) async -> Int {
    // Simulate network delay
    // In a real app, this would be a network call
    return 271828
}

// Function that calls an async function
func fetchUsername(from server: String) async -> String {
    let userID = await fetchUserID(from: server)
    // Simulate network delay
    return "User\(userID)"
}

// Task for concurrent code
func connectUser(to server: String) async {
    async let userID = fetchUserID(from: server)
    async let username = fetchUsername(from: server)
    
    let greeting = await "Hello \(username), user ID: \(userID)"
    print(greeting)
}

// Actor to handle concurrency and data isolation
actor BankAccount {
    private var balance: Double
    
    init(initialBalance: Double) {
        balance = initialBalance
    }
    
    func deposit(amount: Double) {
        balance += amount
    }
    
    func withdraw(amount: Double) -> Bool {
        guard balance >= amount else { return false }
        balance -= amount
        return true
    }
    
    func checkBalance() -> Double {
        return balance
    }
}

// Using throws with async
func fetchData(from server: String) async throws -> Data {
    struct Data {
        let value: Int
    }
    
    if server.isEmpty {
        struct NetworkError: Error {}
        throw NetworkError()
    }
    
    return Data(value: 42)
}

// Using TaskGroup for parallel tasks
func downloadAllImages(from servers: [String]) async throws -> [String] {
    var images = [String]()
    
    // This is a makeshift implementation as we can't actually run task groups in this example
    for server in servers {
        let image = "image from \(server)"
        images.append(image)
    }
    
    return images
}

// Continuations for working with callback-based code
func fetchWeatherHistory(for city: String) async -> [Double] {
    // In a real implementation, this would use withCheckedContinuation
    return [22.5, 23.1, 21.9, 24.3]
}
