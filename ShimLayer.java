public class ShimLayer {

    static{
        System.loadLibrary("ShimLayer");
    }

    public static native void run(int id);

    public static void main(String[]args) {
    	try {
        	new ShimLayer().run(Integer.parseInt(args[0]));
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    }

}