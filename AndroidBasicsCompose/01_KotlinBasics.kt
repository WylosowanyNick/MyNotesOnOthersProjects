// ------------------------------------------------------------------
// Zmienne
fun main() {
    val unreadCount = 5 // zmienna stała
    val readCount = 100
    println("You have ${unreadCount + readCount} total messages in your inbox.")
}

fun main() {
    var count = 10 // zmienna nie-stała
    println("You have $count unread messages.")
    count--
    println("You have $count unread messages.")
}

// ------------------------------------------------------------------
// For
fun main() {
    for(i in 0..10) {
        println(i)
    }
}

// ------------------------------------------------------------------
// Funkcje
fun displayAlertMessage(
    operatingSystem: String = "Unknown OS",
    emailId: String
): String {
    return "There is a new sign-in request on $operatingSystem, for your Google Account $emailId."
}
