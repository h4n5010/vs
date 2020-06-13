import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class DaytimeImpl extends UnicastRemoteObject implements Daytime {
    public DaytimeImpl() throws RemoteException{

    }
    public String getDaytime(String s){
        return s+" "+s;
    }
}
