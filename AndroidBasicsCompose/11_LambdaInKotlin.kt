// Zmienna jako funkcja
// Przykłady
// Przykład z lambdą-nullem
// Lambda jako parametr funkcji + trailing lambda syntax
// Funkcja repeat
// ------------------------------------------------------------------
// Zmienna jako funkcja
fun main() {
    val trickFunction = ::trick
}

fun trick() {
    println("No treats!")
}

// ------------------------------------------------------------------
// Przykład
fun main() {
    val trickFunction = trick
    trick()
    trickFunction()
    treat()
}

val trick = {
    println("No treats!")
}

val treat: () -> Unit = {
    println("Have a treat!")
}

// ------------------------------------------------------------------
// Przykład
fun main() {
    val treatFunction = trickOrTreat(false)
    val trickFunction = trickOrTreat(true)
    treatFunction()
    trickFunction()
}

fun trickOrTreat(isTrick: Boolean): () -> Unit {
    if (isTrick) {
        return trick
    } else {
        return treat
    }
}

// ------------------------------------------------------------------
// Przykład
fun main() {
    val coins: (Int) -> String = { quantity ->
        "$quantity quarters"
    }

    // Ewentualnie:
    /*
    val coins: (Int) -> String = {
    "$it quarters"
    }
    */

    val cupcake: (Int) -> String = { // quantity ->
        "Have a cupcake!"
    }

    val treatFunction = trickOrTreat(false, coins)
    val trickFunction = trickOrTreat(true, cupcake)
    treatFunction()
    trickFunction()
}

fun trickOrTreat(isTrick: Boolean, extraTreat: (Int) -> String): () -> Unit {
    if (isTrick) {
        return trick
    } else {
        println(extraTreat(5))
        return treat
    }
}

// ------------------------------------------------------------------
// Przykład z lambdą-nullem
fun main() {
    val coins: (Int) -> String = { quantity ->
        "$quantity quarters"
    }

    // Ewentualnie:
    /*
    val coins: (Int) -> String = {
    "$it quarters"
    }
    */

    val treatFunction = trickOrTreat(false, coins)
    val trickFunction = trickOrTreat(true, null)
    treatFunction()
    trickFunction()
}

fun trickOrTreat(isTrick: Boolean, extraTreat: ((Int) -> String)?): () -> Unit {
    if (isTrick) {
        return trick
    } else {
        if (extraTreat != null) {
            println(extraTreat(5))
        }
        return treat
    }
}

// ------------------------------------------------------------------
// Lambda jako parametr funkcji + trailing lambda syntax
fun main() {
    val treatFunction = trickOrTreat(false, { "$it quarters" })
    // Ewentualnie (trailing lambda syntax):
    // val treatFunction = trickOrTreat(false) { "$it quarters" }
    val trickFunction = trickOrTreat(true, null)
    treatFunction()
    trickFunction()
}

// ------------------------------------------------------------------
// Funkcja repeat
// Sygnatura
repeat(times: Int, action: (Int) -> Unit) // (Int) jest numerem iteracji

fun main() { // Przykład użycia bez użycia (Int)
    val treatFunction = trickOrTreat(false) { "$it quarters" }
    val trickFunction = trickOrTreat(true, null)
    repeat(4) {
        treatFunction()
    }
    trickFunction()
}