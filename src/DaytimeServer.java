import java.rmi.Naming;
import java.rmi.Remote;

public class DaytimeServer {
    public static void main(String[] args) throws Exception{
        // Start the implementation of the server
        Remote remote = new DaytimeImpl();

        // Write the remote object to the RMI Registry with the name "daytime"
        Naming.rebind("daytime", remote);

        System.out.println("DaytimeServer gestartet...");
    }
}
