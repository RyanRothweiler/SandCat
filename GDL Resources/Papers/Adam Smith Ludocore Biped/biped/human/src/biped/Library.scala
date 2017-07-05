package biped

import java.awt._
import java.awt.geom._

import javax.swing._
import javax.swing.event._

import com.sun.scenario.scenegraph._
import com.sun.scenario.scenegraph.event._

object Library {

    implicit private def d2f(d:Double):Float = d.asInstanceOf[Float]

    private val font = new Font(Font.SANS_SERIF,Font.BOLD,16)
    private val textColor = Color.BLACK
    private val borderColor = Color.BLACK
    private val borderStroke = new BasicStroke(2)

    private def fadeColor(color: Color) = new Color(
            128 + color.getRed / 2,
            128 + color.getGreen / 2,
            128 + color.getBlue / 2)

    def makePathGlyph(src: SGNode, dst: SGNode): SGNode = {
        def coords(node:SGNode)  =
            (node.getBounds().getCenterX, node.getBounds().getCenterY)

        val w = 15

        val line = new SGShape()
        line setDrawPaint borderColor
        line setDrawStroke
            new BasicStroke(w, BasicStroke.CAP_ROUND, BasicStroke.CAP_ROUND)
        line setMode SGAbstractShape.Mode.STROKE
        line setAntialiasingHint RenderingHints.VALUE_ANTIALIAS_ON

        def updateEndpoints() {
            val (sx, sy) = coords(src)
            val (dx, dy) = coords(dst)
            line setShape new Line2D.Double(sx, sy, dx, dy)
        }

        updateEndpoints()

        val listener = new SGNodeListener() {
            def boundsChanged(e: SGNodeEvent) { updateEndpoints() }
        }

        src addNodeListener listener
        dst addNodeListener listener

        val composite = new SGComposite()
        composite setOpacity 0.5f
        composite setChild line

        composite
    }

    def makeTokenGlyph(name: String): SGNode = {
        val text = new SGText()
        text setText name
        text setFillPaint textColor
        text setFont font
        text setAntialiasingHint RenderingHints.VALUE_TEXT_ANTIALIAS_ON

        Script.lookup1("ui_token_label", name) foreach {s => 
          text setText stripQuotes(s)
        }

        val s = 15
        val w = text.getBounds().getWidth + s
        val h = text.getBounds().getHeight + s

        var color = Color.YELLOW
        Script.lookup1("ui_token_color", name) foreach {s =>
          color = fadeColor(Color.decode(s))
        }

        val rect = new SGShape()
        rect setShape new RoundRectangle2D.Double(0,0,w,h,s,s)
        rect setFillPaint new GradientPaint(0,0,Color.WHITE,0,h,fadeColor(color))
        rect setDrawPaint borderColor
        rect setDrawStroke borderStroke
        rect setMode SGAbstractShape.Mode.STROKE_FILL
        rect setAntialiasingHint RenderingHints.VALUE_ANTIALIAS_ON

        var image:SGImage = null
        Script.lookup1("ui_token_image", name) foreach { loc =>
          image = new SGImage()
          image.setImage(
            Main.scene.getPanel.getToolkit.getImage(new java.net.URL(stripQuotes(loc))))
        }

        if(image != null) {
          centeringTransform(image)
        } else {
          centeringTransform(rect, text)
        }
    }

    def makeSpaceGlyph(name: String): SGNode = {
        val text = new SGText()
        text setText name
        text setFillPaint textColor
        text setFont font
        text setAntialiasingHint RenderingHints.VALUE_TEXT_ANTIALIAS_ON

        Script.lookup1("ui_space_label", name) foreach {s =>
          text setText stripQuotes(s)
        }

        var d = (((text.getBounds().getWidth) max (text.getBounds().getHeight)) + 15) max 50
        Script.lookup1("ui_space_size", name) foreach {s =>
          d = Integer.parseInt(s)
        }


        var color = Color.GREEN
        Script.lookup1("ui_space_color", name) foreach {s =>
          color = fadeColor(Color.decode(s))
        }

        val rect = new SGShape()
        rect setShape new Ellipse2D.Double(0,0,d,d)
        rect setFillPaint new GradientPaint(0,0,Color.WHITE,0,d,fadeColor(color))
        rect setDrawPaint borderColor
        rect setDrawStroke borderStroke
        rect setMode SGAbstractShape.Mode.STROKE_FILL
        rect setAntialiasingHint RenderingHints.VALUE_ANTIALIAS_ON

        var image:SGImage = null
        Script.lookup1("ui_space_image", name) foreach { loc =>
          image = new SGImage()
          image.setImage(
            Main.scene.getPanel.getToolkit.getImage(new java.net.URL(stripQuotes(loc))))
        }

        if(image != null) {
          centeringTransform(image)
        } else {
          centeringTransform(rect, text)
        }
    }

    def makeTitleGlyph(title: String): SGNode = {
        var color = Color.YELLOW
        Script.call1("ui_title_color") foreach {s =>
          color = fadeColor(Color.decode(s))
        }

        val s = 150
        val titleFont = new Font(Font.SANS_SERIF, Font.BOLD, s)
        val text = new SGText()
        text setFont titleFont
        text setText stripQuotes(title)
        text setFillPaint new GradientPaint(0,s/2,fadeColor(color),0,s,Color.BLACK,true)
        text setDrawPaint borderColor
        text setDrawStroke new BasicStroke(2)
        text setMode SGAbstractShape.Mode.STROKE_FILL
        text setAntialiasingHint RenderingHints.VALUE_TEXT_ANTIALIAS_ON

        val node = centeringTransform(text)
        val composite = new SGComposite()
        composite setChild node
        composite setOpacity 0.5f

        composite
    }

    trait InstructionController {
        def setContent(s: String)
    }
    def makeInstructionGlyph(prose: String, collect: InstructionController=>Unit): SGNode = {
        val textPane = new JTextPane with InstructionController {
          def setContent(details: String) =
            setText (stripQuotes(prose)+"\n\n"+stripQuotes(details))
        }
        textPane setText stripQuotes(prose)
        textPane setFont new Font(Font.SANS_SERIF, Font.PLAIN, 12)
        textPane setForeground Color.WHITE
        textPane setBackground Color.BLACK
        textPane setEditable false

        collect(textPane)

        val comp = new SGComponent()
        comp setComponent textPane


        val rect = new SGShape()
        rect setFillPaint Color.BLACK
        rect setDrawPaint borderColor
        rect setDrawStroke borderStroke
        rect setMode SGAbstractShape.Mode.STROKE_FILL
        rect setAntialiasingHint RenderingHints.VALUE_ANTIALIAS_ON

        def updateSize {
            val s = 20
            val w = comp.getBounds().getWidth + s
            val h = comp.getBounds().getHeight + s
            rect setShape new RoundRectangle2D.Double(0,0,w,h,s,s)
        }

        updateSize

        comp addNodeListener new SGNodeListener() {
            def boundsChanged(e: SGNodeEvent) = updateSize
        }

        val node = centeringTransform(rect, comp)
        val composite = new SGComposite()
        composite setChild node
        composite setOpacity 0.75f

        composite
    }


    lazy val timer = new java.util.Timer()

    def centeringTransform(node: SGNode): SGNode = {
      val translate = SGTransform.createTranslation(0,0, node)
      def update {
        val b = node.getBounds
        translate.setTranslation(
            -b.getMinX - b.getWidth / 2,
            -b.getMinY - b.getHeight / 2)
      }
      node.addNodeListener(new SGNodeListener() {
        def boundsChanged(e: SGNodeEvent) = update
      })
      timer.schedule(new java.util.TimerTask(){def run = update}, 250) // XXX: account for silent bounds changes
      update
      translate
    }

    def centeringTransform(nodes: SGNode*): SGNode = {
        val group = new SGGroup()
        for (node <- nodes) group.add(centeringTransform(node))
        centeringTransform(group)
    }

    def stripQuotes(s: String) =
      s.replace('\'',' ').trim.replace("\\n","\n")
}
