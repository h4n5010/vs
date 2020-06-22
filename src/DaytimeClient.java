import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.sql.Timestamp;
import java.util.Date;

public class DaytimeClient {
    public static void main(String[] args) throws Exception{
        // Localhost
        String host = args[0];

        // Get the start time of the client and print to console
        Date date = new Date();
        long time = date.getTime();
        Timestamp ts = new Timestamp(time);
        String timestamp = ts.toString();
        System.out.println("Local start time is " + timestamp);

        // Get the current time of the server by RMI and print to console
        try{
            Daytime remote = (Daytime) Naming.lookup("//" + host + "/daytime");
            String received = remote.getDaytime();
            System.out.println("Received time is " + received);
        }
        catch (NotBoundException e){
            System.out.println("NotBoundException: Maybe the server isn't started yet?\n");
            e.printStackTrace();
        }


        // Get the end time of the client and print to console
        time = date.getTime();
        ts = new Timestamp(time);
        timestamp = ts.toString();
        System.out.println("Local endtime is " + timestamp);

    }
}
