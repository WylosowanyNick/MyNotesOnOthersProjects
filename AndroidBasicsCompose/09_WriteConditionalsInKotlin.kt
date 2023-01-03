// if/else, when, null
// ------------------------------------------------------------------
// if/else
fun main() {
    val trafficLightColor = "Black"

    if (trafficLightColor == "Red") {
        println("Stop")
    } else if (trafficLightColor == "Yellow") {
        println("Slow")
    } else if (trafficLightColor == "Green") {
        println("Go")
    } else {
        println("Invalid traffic-light color")
    }

}

fun main() {
    val trafficLightColor = "Black"

    val message =
      if (trafficLightColor == "Red") "Stop"
      else if (trafficLightColor == "Yellow") "Slow"
      else if (trafficLightColor == "Green") "Go"
      else "Invalid traffic-light color"

    println(message)
}

// ------------------------------------------------------------------
// when
fun main() {
    val x: Any = 4

    when (x) {
        2, 3, 5, 7 -> println("x is a prime number between 1 and 10.")
        in 1..10 -> println("x is a number between 1 and 10, but not a prime number.")
        is Int -> println("x is an integer number, but not between 1 and 10.")
        else -> println("x isn't a prime number between 1 and 10.")
    }
}

fun main() {
    val trafficLightColor = "Amber"

    val message = when(trafficLightColor) {
        "Red" -> "Stop"
        "Yellow", "Amber" -> "Proceed with caution."
        "Green" -> "Go"
        else -> "Invalid traffic-light color"
    }
    println(message)
}

// ------------------------------------------------------------------
// null
fun main() {
    var favoriteActor: String? = "Sandra Oh" // '?' umożliwia przypisanie "wartości" null zmiennej
    favoriteActor = null
}

fun main() {
    var favoriteActor: String? = null
    println(favoriteActor?.length) // obsługa nulli z wykorzystaniem '?.'
    // W przypadku, gdy favoriteActor = null, metoda favoriteActor?.length zwraca null
}

fun main() {
    var favoriteActor: String? = null
    println(favoriteActor!!.length) // obsługa nulli z wykorzystaniem '!!.'
    // W przypadku, gdy favoriteActor = null, metoda favoriteActor!!.length wyrzuca wyjątek, który może być obsłużony
}

fun main() {
    var favoriteActor: String? = null

    if(favoriteActor != null) { // obsługa nulli z wykorzystaniem if/else.
      println("The number of characters in your favorite actor's name is ${favoriteActor.length}.")
      // W tym bloku nie trzeba wykorzystywać operatorów '?.'/'!!.'. Podobnie jest dla favoriteActor == null
    } else {
      println("You didn't input a name.")
    }
}

fun main() {
    val favoriteActor: String? = "Sandra Oh"

    val lengthOfName = if(favoriteActor != null) {
      favoriteActor.length
    } else {
      0
    }

    println("The number of characters in your favorite actor's name is $lengthOfName.")
}

fun main() {
    val favoriteActor: String? = "Sandra Oh"

    val lengthOfName = favoriteActor?.length ?: 0
    // favoriteActor?.length = null -> zwracane jest 0

    println("The number of characters in your favorite actor's name is $lengthOfName.")
}