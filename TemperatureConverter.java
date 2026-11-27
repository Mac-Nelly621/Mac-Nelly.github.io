import java.util.Scanner;

public class TemperatureConverter {
    public static void main(String[] args) {

        Scanner input = new Scanner(System.in);

        // Ask user for Fahrenheit value
        System.out.print("Enter temperature in Fahrenheit: ");
        double fahrenheit = input.nextDouble();

        // Convert to Celsius
        double celsius = (fahrenheit - 32) * 5 / 9;

        // Display result
        System.out.println("Temperature in Celsius: " + celsius);

        input.close();
    }
}