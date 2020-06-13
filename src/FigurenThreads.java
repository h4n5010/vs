public class FigurenThreads {
    public static void main(String[] args){
        Figur f1 = new ConcreteFigur();

        Thread reader = new Thread(new Leser(f1)); // Leser fuer Schachfigur f1
        Thread writer = new Thread(new Schreiber(f1)); // Schreiber fuer Schachfigur f1
        writer.setDaemon(true); // Reader daemon true setzen, damit er im Hintergrund ausgefuehrt wird

        // Threads starten
        writer.start();
        reader.start();
    }
}
