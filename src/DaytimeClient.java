import java.rmi.Naming;
import java.sql.Timestamp;
import java.util.Date;

public class DaytimeClient {
    public static void main(String[] args) throws Exception{
        String host = args[0];
        //String host = "Test"; // Nur für Debugging selbst gesetzt, kann kein argument in intellij übergeben

        Date date = new Date();
        long time = date.getTime();
        Timestamp ts = new Timestamp(time);
        String timestamp = ts.toString();
        System.out.println("Local start time is " + timestamp);
        Daytime remote = (Daytime) Naming.lookup("//" + host + "/localhost");
        String received = remote.getDaytime();
        System.out.println("Received time is " + received);
        time = date.getTime();
        ts = new Timestamp(time);
        timestamp = ts.toString();
        System.out.println("Local endtime is " + timestamp);
    }
}
