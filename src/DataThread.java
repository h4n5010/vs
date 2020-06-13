import java.io.IOException;

public class DataThread extends Thread {

    private UniqueId data;
    private int result;
    private String name;

    public DataThread(UniqueId data, String name){
        this.data = data;
        this.name = name;
    }
    public void run(){
        for(int i = 0; i < 10; i++){
            try{
                result = data.getNext();
            } catch (IOException e){
                //e.printStackTrace();
                System.out.println("IOException von " + name);
            }
            System.out.println("Return: " + result + " Name: " + name);
        }

    }
}
