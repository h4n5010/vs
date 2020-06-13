public class ConcreteFigur extends Figur {

    // Neue Koordinaten fuer Schachfigur setzen
    public synchronized void setPosition(char x, int y) {
        this.x = x;
        MachMal.eineZehntelSekundeLangGarNichts(); // 10 Millisekunden warten gemaeß Aufgabenstellung
        this.y = y;
    }

    // Gibt die aktuelle Position der Schachfigur als String zurück
    public synchronized String getPosition(){
        for(int i = 0; i < 10; i++) { // Eine Sekunde lang warten gemaeß Aufgabenstellung
            MachMal.eineZehntelSekundeLangGarNichts();
        }
        return "" + x + y;
    }
}
