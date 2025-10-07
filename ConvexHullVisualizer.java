import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

public class ConvexHullVisualizer extends JPanel {
    private final ArrayList<Point> points = new ArrayList<>();
    private volatile ArrayList<Point> hull = new ArrayList<>();
    private final JButton clearButton;
    private final Map<Point, String> pointLabels = new HashMap<>();
    private volatile Set<Point> removedPoints = new HashSet<>();
    private int pointCounter = 1;
    private JTextArea logArea; // Result window text area
    private JFrame logFrame;   // Separate window for logs

    public ConvexHullVisualizer() {
        setupLogWindow(); // Create the result window

        addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                Point newPoint = new Point(e.getX(), e.getY());
                points.add(newPoint);
                pointLabels.put(newPoint, "P" + pointCounter++);
                log("Added point: " + pointLabels.get(newPoint) + " at (" + newPoint.x + ", " + newPoint.y + ")");
                new Thread(ConvexHullVisualizer.this::computeConvexHull).start();
            }
        });

        clearButton = new JButton("Clear Screen");
        clearButton.addActionListener(e -> {
            points.clear();
            hull.clear();
            removedPoints.clear();
            pointLabels.clear();
            pointCounter = 1;
            logArea.setText(""); // Clear log window
            repaint();
        });

        setLayout(new FlowLayout(FlowLayout.LEFT));
        add(clearButton);
    }

    private void setupLogWindow() {
        logFrame = new JFrame("Convex Hull Steps");
        logArea = new JTextArea(20, 40);
        logArea.setEditable(false);
        logFrame.add(new JScrollPane(logArea));
        logFrame.setSize(400, 400);
        logFrame.setLocation(810, 100);
        logFrame.setVisible(true);
    }

    private void log(String message) {
        SwingUtilities.invokeLater(() -> {
            logArea.append(message + "\n");
        });
    }

    private void computeConvexHull() {
        if (points.size() < 3) {
            hull = new ArrayList<>(points);
            log("Not enough points to form a hull. Using all available points.");
            repaint();
            return;
        }

        hull.clear();
        removedPoints.clear();
        repaint();
        sleep(500);

        Point maxYPoint = points.stream()
                .max(Comparator.comparingInt((Point p) -> p.y).thenComparingInt(p -> p.x))
                .orElseThrow();

        log("Starting point (highest Y, leftmost X): " + pointLabels.get(maxYPoint));

        points.sort((p1, p2) -> {
            double angle1 = Math.atan2(p1.y - maxYPoint.y, p1.x - maxYPoint.x);
            double angle2 = Math.atan2(p2.y - maxYPoint.y, p2.x - maxYPoint.x);
            return Double.compare(angle2, angle1);
        });

        Stack<Point> stack = new Stack<>();
        stack.push(maxYPoint);

        for (Point p : points) {
            while (stack.size() > 1 && crossProduct(stack.get(stack.size() - 2), stack.peek(), p) >= 0) {
                Point removed = stack.pop();
                removedPoints.add(removed);
                log("Removed: " + pointLabels.get(removed) + " (Did not maintain convex property)");
                updateHull(stack);
            }
            stack.push(p);
            log("Added to hull: " + pointLabels.get(p));
            updateHull(stack);
        }

        stack.push(maxYPoint);
        log("Closed hull with: " + pointLabels.get(maxYPoint));
        updateHull(stack);
    }

    private void updateHull(Stack<Point> stack) {
        ArrayList<Point> newHull = new ArrayList<>(stack);
        SwingUtilities.invokeLater(() -> {
            hull = newHull;
            repaint();
        });
        sleep(500);
    }

    private int crossProduct(Point p1, Point p2, Point p3) {
        return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    }

    private void sleep(int ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException ignored) {}
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        // Draw all points
        for (Point p : points) {
            g.setColor(Color.BLUE);
            g.fillOval(p.x - 3, p.y - 3, 10, 10);
            g.drawString(pointLabels.get(p), p.x + 5, p.y - 5);
        }

        // Draw removed points in Orange
        g.setColor(Color.ORANGE);
        for (Point p : removedPoints) {
            g.fillOval(p.x - 3, p.y - 3, 10, 10);
        }

        // Draw Convex Hull in Green
        if (!hull.isEmpty()) {
            g.setColor(Color.GREEN);
            Point prev = hull.get(0);

            for (int i = 1; i < hull.size(); i++) {
                Point current = hull.get(i);
                g.drawLine(prev.x, prev.y, current.x, current.y);
                prev = current;
            }
        }
    }

    public static void main(String[] args) {
        JFrame frame = new JFrame("Convex Hull Visualizer");
        ConvexHullVisualizer panel = new ConvexHullVisualizer();

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(800, 600);
        frame.add(panel);
        frame.setVisible(true);
    }
}
