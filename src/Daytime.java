import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Daytime extends Remote{
    public String getDaytime(String s) throws RemoteException;
}
