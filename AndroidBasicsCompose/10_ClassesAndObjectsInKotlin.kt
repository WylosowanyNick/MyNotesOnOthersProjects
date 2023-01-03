// Przykładowa definicja klasy i jej instancja w funkcji main
// Getter i Setter
// Przykładowy konstruktor klasy
// Dziedziczenie (IS-A)
// Złożenie klas (HAS-A)
// Nadpisywanie metod/pól dziedziczonej klasy
// Modyfikatory widoczności (private, protected, internal, public)
// Interfejs
// ------------------------------------------------------------------
// Przykładowa definicja klasy i jej instancja w funkcji main
class SmartDevice {

    val name = "Android TV"
    val category = "Entertainment"
    var deviceStatus = "online"

    fun turnOn(){
        println("Smart device is turned on.")
    }

    fun turnOff(){
        println("Smart device is turned off.")
    }
}

fun main(){
    val smartTvDevice = SmartDevice()
    println("Device name is: ${smartTvDevice.name}")
    smartTvDevice.turnOn()
    smartTvDevice.turnOff()
}

// ------------------------------------------------------------------
// Getter i Setter
// Poniższy kod przedstawia automatycznie generowany getter i setter dla pola klasy speakerVolume.
var speakerVolume = 2
    get() = field
    set(value) {
        field = value
    }

// Getter i Setter może zostać zdefiniowany przez użytkownika:
var speakerVolume = 2
    get() = field
    set(value) {
        if (value in 0..100) {
            field = value
        }
    }

// ------------------------------------------------------------------
// Przykładowy konstruktor klasy
class SmartDevice(val name: String, val category: String) { // To jest podstawowy konstruktor
    var deviceStatus = "online"

    constructor(name: String, category: String, statusCode: Int) : this(name, category) { // To jest podrzędny konstruktor
    // Wywołuje on główny konstruktor w 'this(name, category)'
        deviceStatus = when (statusCode) {
            0 -> "offline"
            1 -> "online"
            else -> "unknown"
        }
    }

    fun turnOn(){
        println("Smart device is turned on.")
    }

    fun turnOff(){
        println("Smart device is turned off.")
    }
}

fun main(){
    val smartTvDevice = SmartDevice(name = "Android TV", category = "Entertainment")
    println("Device name is: ${smartTvDevice.name}")
    smartTvDevice.turnOn()
    smartTvDevice.turnOff()
}

// ------------------------------------------------------------------
// Dziedziczenie (IS-A)
open class SmartDevice(val name: String, val category: String) {
    // Słowo open informuje kompilator, że klasę można dziedziczyć
    ...
}

class SmartTvDevice(deviceName: String, deviceCategory: String) :
    SmartDevice(name = deviceName, category = deviceCategory) { // Dziedziczenie

    var speakerVolume = 2
        set(value) {
            if (value in 0..100) {
                field = value
            }
        }
     var channelNumber = 1
        set(value) {
            if (value in 0..200) {
                field = value
            }
        }

    fun increaseSpeakerVolume() {
        speakerVolume++
        println("Speaker volume increased to $speakerVolume.")
    }

    fun nextChannel() {
        channelNumber++
        println("Channel number increased to $channelNumber.")
    }
}

// ------------------------------------------------------------------
// Złożenie klas (HAS-A)
class SmartLightDevice(deviceName: String, deviceCategory: String) :
    SmartDevice(name = deviceName, category = deviceCategory) {

...

}

// The SmartHome class HAS-A smart TV device.
class SmartHome(val smartTvDevice: SmartTvDevice) {

    fun turnOnTv() {
        smartTvDevice.turnOn()
    }

    fun turnOffTv() {
        smartTvDevice.turnOff()
    }

    fun increaseTvVolume() {
        smartTvDevice.increaseSpeakerVolume()
    }

    fun changeTvChannelToNext() {
        smartTvDevice.nextChannel()
    }
}

// ------------------------------------------------------------------
// Nadpisywanie metod/pól dziedziczonej klasy
open class SmartDevice(val name: String, val category: String) { // open -> klasa do dziedziczenia
    var deviceStatus = "online"

    open fun turnOn() { // open -> metoda, którą klasa dziedzicząca może nadpisać
        // ciało funkcji
    }

    open fun turnOff() { // open -> metoda, którą klasa dziedzicząca może nadpisać
        // ciało funkcji
    }
}

class SmartLightDevice(name: String, category: String) :
    SmartDevice(name = name, category = category) {

    var brightnessLevel = 0

    override fun turnOn() { // override -> nadpisanie metody rodzica
        deviceStatus = "on"
        brightnessLevel = 2
        println("$name turned on. The brightness level is $brightnessLevel.")
    }

    override fun turnOff() { // override -> nadpisanie metody rodzica
        deviceStatus = "off"
        brightnessLevel = 0
        println("Smart Light turned off")
    }

    fun increaseBrightness() {
        brightnessLevel++
    }
}

fun main(){
    var smartLightDevice : SmartLightDevice = SmartLightDevice("Android TV", "Entertainment")
    smartLightDevice.turnOn()
}

// ------------------------------------------------------------------
// Modyfikatory widoczności (private, protected, internal, public)
class SmartLightDevice(deviceName: String, deviceCategory: String) :
    SmartDevice(name = deviceName, category = deviceCategory) {

    private var brightnessLevel = 0 // dla pól
        set(value) {
            if (value in 0..100) {
                field = value
            }
        }

    ...
}

open class SmartDevice(val name: String, val category: String) {

    ...
    var deviceStatus = "online" // domyślnie public
        protected set(value){ // dla settera pola
           field = value
       }

    ...
}

open class SmartDevice(val name: String, val category: String) {

    ...
    var deviceStatus = "online"
        protected set // dla domyślnego settera pola
    ...
}

class SmartTvDevice(deviceName: String, deviceCategory: String) :
    SmartDevice(name = deviceName, category = deviceCategory) {

    ...
    protected fun nextChannel() { // dla metody
        channelNumber++
    }
    ...
}

open class SmartDevice protected constructor (val name: String, val category: String) { // dla głównego konstruktora

    ...

}

internal open class SmartDevice(val name: String, val category: String) { // dla klas

    ...

}

// ------------------------------------------------------------------
// Interfejs
import kotlin.properties.ReadWriteProperty
import kotlin.reflect.KProperty

class RangeRegulator(
    initialValue: Int,
    private val minValue: Int,
    private val maxValue: Int
) : ReadWriteProperty<Any?, Int> { // dziedziczenie interfejsu
    var fieldData = initialValue

    override fun getValue(thisRef: Any?, property: KProperty<*>): Int { // nadpisanie (obowiązkowej) metody interfejsu
        return fieldData
    }

    override fun setValue(thisRef: Any?, property: KProperty<*>, value: Int) { // nadpisanie (obowiązkowej) metody interfejsu
        if (value in minValue..maxValue) {
            fieldData = value
        }
    }
}

class SmartTvDevice(deviceName: String, deviceCategory: String) :
    SmartDevice(name = deviceName, category = deviceCategory) {

    var speakerVolume by RangeRegulator(initialValue = 0, minValue = 0, maxValue = 100) // wykorzystanie interfejsu

    var channelNumber by RangeRegulator(initialValue = 1, minValue = 0, maxValue = 200) // wykorzystanie interfejsu

    ...
}