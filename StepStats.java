public class StepStats {
    // private data field 
    private int thousands;
    private int hundreds;
    private int tens;
    
    // constructor 
    public StepStats(int thousands, int hundreds, int tens) {
        this.thousands = thousands;
        this.hundreds = hundreds;
        this.tens = tens;
    }
    String[] parts = new String[3];
    int count = 0;
if (thousands > 0) {
    parts[count++] = thousands + (thousands == 1 ? " thousand" : " thousands");
}
if (hundreds > 0) {
    parts[count++] = hundreds + (hundreds == 1 ? " hundred" : " hundreds");
}    
if (tens > 0) {
    parts[count++] = tens + (tens == 1 ? " ten" : " tens");
}
if (count == 0) {
    system.out.println("0 steps");
    return;
}
StringBuilder result = new StringBuilder();
if (count == 1) {
    sb.append(parts[0]);
} else if (count == 2) {
    sb.append(parts[0]).append(" and ").append(parts[1]);
} else {
    sb.append(parts[0]).append(", ").append(parts[1]).append(" and ").append(parts[2]);

}
system.out.println(sb.toString());
}
// Getters
public int getThousands() {
    return thousands;
}
public int getHundreds() {
    return hundreds;
}
public int getTens() {
    return tens;
}
}