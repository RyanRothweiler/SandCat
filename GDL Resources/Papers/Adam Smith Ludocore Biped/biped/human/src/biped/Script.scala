package biped

import alice.tuprolog._
import alice.tuprolog.event._

object Script {
  val prolog = new Prolog()

  prolog.addTheory(
    new Theory(
      getClass().getResourceAsStream("runtime.pl")))

  implicit private def set[T](seq: Seq[T]): Set[T] = Set[T]() ++ seq

  def load(code: String) {
    prolog.addTheory(new Theory(code))
  }

  def load(is: java.io.InputStream) {
    prolog.addTheory(new Theory(is))
  }

  def call0(pred: String): Option[Unit] = {
    val query = new Struct(pred)
    prolog.solve(query) isSuccess match {
      case true => Some()
      case false => None
    }
  }

  def call1(pred: String): Option[String] = {
    val query = new Struct(pred, new Var("X"))
    val solution = prolog.solve(query)
    solution isSuccess match {
      case true => Some(solution.getVarValue("X").toString)
      case false => None
    }
  }

  def lookup1(pred: String, key: String): Option[String] = {
    val query = new Struct(pred, prolog.toTerm(key), new Var("V"))
    val solution = prolog.solve(query)
    solution isSuccess match {
      case true => Some(solution.getVarValue("V").toString)
      case false => None
    }
  }

  def lookup2(pred: String, keyA: String, keyB: String): Option[String] = {
    val query = new Struct(pred, prolog.toTerm(keyA), prolog.toTerm(keyB), new Var("V"))
    val solution = prolog.solve(query)
    solution isSuccess match {
      case true => Some(solution.getVarValue("V").toString)
      case false => None
    }
  }

  def setof1(pred: String): Set[String] = {
    val template = new Struct(pred, new Var("X"))
    val query = new Struct("setof", template, template, new Var("Xs"))
    val solution = prolog.solve(query)
    solution isSuccess match {
        case true => {
          val xs = solution.getVarValue("Xs")
          for(x <- untree(xs)) yield x.getTerm match {
            case s:Struct => s.getArg(0).toString
          }
        }
        case false => Set()
    }
  }

  def setof2(pred: String): Set[(String,String)] = {
    val template = new Struct(pred, new Var("X"), new Var("Y"))
    val query = new Struct("setof", template, template, new Var("XYs"))
    val solution = prolog.solve(query)
    solution isSuccess match {
      case true => {
        val xys = solution.getVarValue("XYs")
        for(xy <- untree(xys)) yield xy.getTerm match {
            case s:Struct =>
              (s.getArg(0).toString, s.getArg(1).toString)
        }
      }
      case false => Set()
    }
  }

  def setof3(pred: String): Set[(String,String,String)] = {
    val template = new Struct(pred, new Var("X"), new Var("Y"), new Var("Z"))
    val query = new Struct("setof", template, template, new Var("XYZs"))
    val solution = prolog.solve(query)
    solution isSuccess match {
      case true => {
        val xyzs = solution.getVarValue("XYZs")
        for(xyz <- untree(xyzs)) yield xyz.getTerm match {
            case s:Struct =>
              (s.getArg(0).toString, s.getArg(1).toString, s.getArg(2).toString)
        }
      }
      case false => Set()
    }
  }

  def assertHappens(functor: String, args: String*):Boolean = {
    val eventSource = functor + (args.length match {
      case 0 => ""
      case _ => args.mkString("(",",",")")
    })
    val interestingQuery = "ui_interesting(" + eventSource + ")."
    val isol = prolog.solve(interestingQuery)
    if (isol isSuccess) {
      val assertQuery = "assertz(happens_spontaneously(" + eventSource + "))."
      prolog.solve(assertQuery)
      true
    } else false
  }

  def retractallHappens() {
    prolog.solve("retractall(happens_spontaneously(_)).")
  }

  def retractallHolds() {
    prolog.solve("retractall(holds_transiently(_)).")
  }

  def incrementTimepoint() {
    prolog.solve("retract(timepoint(T)), T1 is T+1, assertz(timepoint(T1)).")
  }

  def assertallHolds(items: Set[String]) {
    items.size match {
      case 0 => ()
      case _ => {
              val fs = items.mkString("[",",","]")
              val query = "forall(member(F,"+fs+"), assertz(holds_transiently(F)))."
              prolog.solve(query)
      }
    }
  }

  private def untree(term: Term): Seq[Term] = {
    var head = term
    val buf = new collection.mutable.ListBuffer[Term]()
    while(head.isList && !head.isEmptyList) {
      val cell = head.getTerm.asInstanceOf[Struct]
      buf.append(cell.getArg(0))
      head = cell.getArg(1)
    }
    buf.toSeq
  }

}
