import javax.swing.*;
import java.awt.*;

public class BirdFlightPanel extends JPanel {
    private static final int N = 2000;

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        // enable anti‐aliasing
        Graphics2D g2 = (Graphics2D) g;
        g2.setRenderingHint(
            RenderingHints.KEY_ANTIALIASING,
            RenderingHints.VALUE_ANTIALIAS_ON
        );

        int w = getWidth();
        int h = getHeight();

        for (int i = 1; i <= N; i++) {
            // compute normalized math coordinates
            double t = 2 * Math.PI * i / N;
            double x1 = 3 * Math.pow(Math.sin(t), 3);
            double y1 = -Math.cos(8 * Math.PI * i / N);
            double x2 = 1.5 * Math.pow(Math.sin(t), 3);
            double y2 = -0.5 * Math.cos(6 * Math.PI * i / N);

            // map to panel pixels
            int px1 = (int) ((x1 + 3) / 6 * w);
            int py1 = (int) ((1 - (y1 + 1) / 2) * h);
            int px2 = (int) ((x2 + 3) / 6 * w);
            int py2 = (int) ((1 - (y2 + 1) / 2) * h);

            g2.drawLine(px1, py1, px2, py2);
        }
    }

    private static void createAndShowGui() {
        JFrame frame = new JFrame("Bird in Flight");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(new BirdFlightPanel());
        frame.setSize(800, 600);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(BirdFlightPanel::createAndShowGui);
    }
}
