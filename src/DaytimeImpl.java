import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.sql.Timestamp;
import java.util.Date;

public class DaytimeImpl extends UnicastRemoteObject implements Remote {
    public DaytimeImpl() throws RemoteException{

    }
    public String getDaytime(){
        Date date = new Date();
        long time = date.getTime();
        Timestamp ts = new Timestamp(time);
        String timestamp = ts.toString();
        return timestamp;
    }
}
