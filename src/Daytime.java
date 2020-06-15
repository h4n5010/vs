import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Daytime extends Remote{
    public String getDaytime() throws RemoteException;
}
