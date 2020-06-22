import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.sql.Timestamp;
import java.util.Date;

public class DaytimeImpl extends UnicastRemoteObject implements Daytime {
    public DaytimeImpl() throws RemoteException{

    }
    @Override
    public String getDaytime(){
        // Create a date object, get the current time in milliseconds and convert them to a Timestamp Object
        Date date = new Date();
        long time = date.getTime();
        Timestamp ts = new Timestamp(time);

        // Return the timestamp as String to the client
        return ts.toString();

    }

    // Test, if class DaytimeImpl knows Daytime
    public static void main(String[] args){
        try{
            Daytime test = new DaytimeImpl();
            test.getDaytime();
        } catch (Exception e){
            e.printStackTrace();
        }
        System.out.println("Fertig");

    }
}
