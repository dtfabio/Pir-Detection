package pir;

import jssc.SerialPort;
import jssc.SerialPortException;
import javafx.concurrent.Task;

public class SerialHandler extends Task<Integer> {
    private boolean arduinoCollegato = false;
    private boolean portaAperta = false;

    private static SerialHandler instance = null;

    private static SerialPort serialPort = new SerialPort("COM4");

    private SerialHandler() {
    }

    /**
     * Metodo che implementa il design pattern Singleton
     * @return L'istanza della classe
     */
    public static SerialHandler getInstance() {
        if (instance == null) {
            instance = new SerialHandler();
        }
        return instance;
    }

    @Override
    protected Integer call() {
        String buffer;

        while (!isCancelled()) {
            if (isArduinoCollegato() && !isPortaAperta()) {
                try {
                    serialPort.openPort();

                    serialPort.setParams(SerialPort.BAUDRATE_9600,
                            SerialPort.DATABITS_8,
                            SerialPort.STOPBITS_1,
                            SerialPort.PARITY_NONE,
                            true, false);

                    serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN
                            | SerialPort.FLOWCONTROL_RTSCTS_OUT);

                    setPortaAperta(true);
                    ControlloIlluminazione.getInstance().spegniLEDArduino();
                } catch (SerialPortException ex) {
                    Logger.getLogger(SerialHandler.class.getName()).log(Level.SEVERE, null, ex);
                    setArduinoCollegato(false);
                }
            }

            try {
                // Addormenta il thread per 0.2 millisecondi
                Thread.sleep(0, 200000);
            } catch (InterruptedException ex) {
                Logger.getLogger(SerialHandler.class.getName()).log(Level.SEVERE, null, ex);
            }

            // Operazioni da eseguire solo se la porta seriale e' aperta
            if (isPortaAperta()) {
                // Leggi il buffer
                buffer = null;
                try {
                    buffer = serialPort.readString(); // Leggi dati dalla porta seriale
                } catch (SerialPortException ex) {
                    Logger.getLogger(SerialHandler.class.getName()).log(Level.SEVERE, null, ex);
                }
                if (buffer != null) {
                    parseBuffer(buffer);
                }
            }

            ControlloPAI.getInstance().eseguiOperazioniPAI();
            ControlloTraffico.getInstance().eseguiOperazioniTraffico();
            ControlloIlluminazione.getInstance().eseguiOperazioniIlluminazione();
        }

        return 0;
    }

    /**
     * Invia dati alla porta seriale verso Arduino. Questi dati avranno il formato "&lt;x,y,&gt;".
     * @param valore1 valore x.
     * @param valore2 valore y.
     */
    public void writeOnSerialPort(int valore1, int valore2) {
        String serialString;

        if (isPortaAperta()) {
            try {
                serialString = "<" + Integer.toString(valore1) + "," +
                        Integer.toString(valore2) + ",>";
                serialPort.writeString(serialString);
            } catch (SerialPortException ex) {
                Logger.getLogger(SerialHandler.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    private void leggiInputSeriale(String stringa) {
        if (stringa.equals("0")) {
            // IR1 non e' occluso
            IR.getInstance().ricevutoInputIR1Occluso(false);
        }
        else if (stringa.equals("1")) {
            // IR1 e' occluso
            IR.getInstance().ricevutoInputIR1Occluso(true);
        }
        else if (stringa.equals("2")) {
            // IR2 non e' occluso
            IR.getInstance().ricevutoInputIR2Occluso(false);
        }
        else if (stringa.equals("3")) {
            // IR2 e' occluso
            IR.getInstance().ricevutoInputIR2Occluso(true);
        }
        else if (stringa.equals("4")) {
            ControlloPAI.getInstance().ricevutoInputTemperaturaBassa();
        }
        else if (stringa.equals("5")) {
            ControlloPAI.getInstance().ricevutoInputTemperaturaAlta();
        }
        else if (stringa.equals("6")) {
            System.out.println("OK");
        }
        else if (stringa.equals("7")) {
            System.out.println("Errore");
        }
    }

    private void parseBuffer(String buffer) {
        char[] begin = new char[1];
        char[] end = new char[buffer.length() - 1];

        buffer.getChars(0, 1, begin, 0);
        buffer.getChars(1, buffer.length(), end, 0);

        //System.out.println(begin);
        leggiInputSeriale(String.valueOf(begin));

        if (String.valueOf(end).length() > 0) {
            parseBuffer(String.valueOf(end));
        }
    }


    public boolean isPortaAperta() {
        return portaAperta;
    }

    public void setPortaAperta(boolean portaAperta) {
        this.portaAperta = portaAperta;
    }

    public boolean isArduinoCollegato() {
        return arduinoCollegato;
    }

    public void setArduinoCollegato(boolean arduinoCollegato) {
        this.arduinoCollegato = arduinoCollegato;
    }
}
