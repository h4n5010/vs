import java.rmi.Naming;
import java.rmi.Remote;

public class DaytimeServer {
    public static void main(String[] args) throws Exception{
        Remote remote = new DaytimeImpl();
        Naming.rebind("localhost", remote);
        System.out.println("DaytimeServer gestartet...");
    }
}
