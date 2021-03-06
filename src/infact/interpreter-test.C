/// \file
/// Test driver for the Interpreter class.
/// \author dbikel@google.com (Dan Bikel)

#include <iostream>
#include <memory>

#include "interpreter.H"

using namespace std;
using namespace infact;

int
main(int argc, char **argv) {
  cout << "Here is a list of abstract types and the concrete implementations\n"
       << "you can construct:" << endl;

  int debug = 0;
  cout << "Debug level: " << debug << endl;

  Interpreter interpreter(debug);

  cout << endl;
  interpreter.PrintFactories(cout);

  cout << "\nHello!  Please type assignment statements.\n" << endl;

  if (argc >= 2) {
    interpreter.Eval(argv[1]);
  } else {
    interpreter.Eval(cin);
  }

  cout << "\nNow doing some hard-coded testing, looking to see if you\n"
       << "set a variable named \"f\" to have a boolean value." << endl;

  bool value_for_f;
  bool success = interpreter.Get("f", &value_for_f);
  if (success) {
    cout << "Success! f=" << (value_for_f ? "true" : "false") << endl;
  } else {
    cout << ":( ... no boolean value for variable f" << endl;
  }

  cout << "\n\nEnvironment: " << endl;
  interpreter.PrintEnv(cout);

  cout << "\nHave a nice day!\n" << endl;
}

/// \mainpage InFact Framework
/// \section welcome_sec Welcome to the InFact Framework!
/// InFact makes it easy to construct C++ objects at run-time, using a
/// simple-yet-powerful language that is almost identical to C++.
///
/// \section contents_sec Contents
/// <ul>
///   <li> \ref cpp_example
///   <li> \ref details
///   <li> \ref language
///   <li> \ref putting_together
/// </ul>
///
/// \section Dynamic object instantiation
///
/// There&rsquo;s a famous quotation of Philip Greenspun known as <a
/// href="http://en.wikipedia.org/wiki/Greenspun's_Tenth_Rule">Greenspun&rsquo;s
/// Tenth Rule</a>:
/// \par Greenspun&rsquo;s Tenth Rule
/// Any sufficiently complicated C or Fortran program contains an ad
/// hoc, informally-specified, bug-ridden, slow implementation of half
/// of Common Lisp.
///
/// This statement is remarkably true in practice, and no less so
/// here.  C++ lacks convenient support for dynamic object
/// instantiation, but the InFact Framework uses  a \link infact::Factory
/// Factory \endlink class and a C++-style (yet simple) syntax.
///
/// \subsection cpp_example An example: The way C++ does it
///
/// To motivate the C++-style syntax used by the InFact Framework&rsquo;s
/// \link infact::Factory Factory \endlink class, let&rsquo;s look
/// at a simple example of a C++ class <tt>Person</tt> and its constructor:
/// \code
/// // A class to represent a date in the standard Gregorian calendar.
/// class Date {
///  public:
///    Date(int year, int month, int day) :
///      year_(year), month_(month), day_(day) { }
///  private:
///    int year_;
///    int month_;
///    int day_;
/// };
///
/// // A class to represent a few facts about a person.
/// class Person {
///  public:
///    Person(const string &name, int cm_height, const Date &birthday) :
///      name_(name), cm_height_(cm_height), birthday_(birthday) { }
///  private:
///   string name_;
///   int cm_height_;
///   Date birthday_;
/// };
/// \endcode
/// As you can see, the <tt>Person</tt> class has three data members,
/// one of which happens to be an instance of another class called
/// <tt>Date</tt>.  In this case, all of the initialization of a
/// <tt>Person</tt> happens in the <i>initialization phase</i> of the
/// constructor&mdash;the part after the colon but before the
/// <i>declaration phase</i> block.  By convention, each parameter to
/// the constructor has a name nearly identical to the data member
/// that will be initialized from it. If we wanted to construct a
/// <tt>Person</tt> instance for someone named &ldquo;Fred&rdquo; who
/// was 180 cm tall and was born January 10th, 1990, we could write
/// the following:
/// \code
///   Person fred("Fred", 180, Date(1990, 1, 10));
/// \endcode
///
/// If <tt>Person</tt> were a \link infact::Factory
/// Factory\endlink-constructible type in the InFact Framework,
/// we would be able to specify the following as a <i>specification string</i>
/// to tell the \link infact::Factory Factory \endlink how to
/// construct a <tt>Person</tt> instance for Fred:
/// \code
///   Person(name("Fred"), cm_height(180), birthday(Date(year(1990), month(1), day(10))))
/// \endcode
/// As you can see, the syntax is very similar to that of
/// C++. It&rsquo;s kind of a combination of the parameter list and
/// the initialization phase of a C++ constructor.  Unfortunately, we
/// can&rsquo;t get this kind of dynamic instantiation in C++ for
/// free; we need some help from the programmer.  However, we&rsquo;ve
/// tried to make the burden on the programmer fairly low, using just
/// a couple of macros to help declare a \link infact::Factory
/// Factory \endlink for an abstract base class, as well as to make it
/// easy to make that \link infact::Factory Factory \endlink aware of
/// the concrete subtypes of that base class that it can construct.
///
/// \subsection details Some nitty gritty details: declaring factories for abstract types and registering concrete subtypes
///
/// Every \link infact::Factory Factory\endlink-constructible
/// abstract type needs to declare its factory via the \link
/// IMPLEMENT_FACTORY \endlink macro.  For example, since the InFact
/// Framework uses a \link infact::Factory Factory \endlink to
/// construct concrete instances of the abstract type \link
/// infact::Animal Animal \endlink, the line
/// \code
///   IMPLEMENT_FACTORY(Animal)
/// \endcode
/// appears in the file <tt>feature-extractor.C</tt>.  (It is
/// unfortunate that we have to resort to using macros, but the point
/// is that the burden on the programmer to create a factory is
/// extremely low, and therefore so is the risk of introducing bugs.)
///
/// By convention every \link infact::Factory
/// Factory\endlink-constructible abstract type defines one or two
/// macros in terms of the \link REGISTER_NAMED \endlink macro defined
/// in \link factory.H\endlink to allow concrete subtypes to register
/// themselves with the \link infact::Factory Factory\endlink, so
/// that they may be instantiated.  For example, since the \link
/// infact::Animal Animal \endlink class is an
/// abstract base class in the InFact Framework that has a \link
/// infact::Factory Factory\endlink, in feature-extractor.H you
/// can find the declaration of a macro, \link
/// REGISTER_ANIMAL \endlink.  The \link
/// infact::Cow Cow \endlink
/// class is a concrete subclass of \link infact::Animal
/// Animal\endlink, and so it registers itself with \link
/// infact::Factory Factory\endlink\<\link
/// infact::Animal Animal\endlink\> by having
/// \code REGISTER_ANIMAL(Cow) \endcode
/// in <tt>cow.C</tt>. That macro expands to \code
/// REGISTER_NAMED(Cow, Cow,
/// Animal) \endcode which tells the \link infact::Factory
/// Factory\endlink\<Animal\> that there is a class
/// <tt>Cow</tt> whose &ldquo;factory name&rdquo;
/// (the string that can appear in <i>specification
/// strings</i>&mdash;more on these in a moment) is
/// <tt>&quot;Cow&quot;</tt> and that the class
/// \link infact::Cow Cow
/// \endlink is a concrete subclass of \link
/// infact::Animal Animal \endlink, <i>i.e.</i>,
/// that it can be constructed by
/// <tt>Factory\<Animal\></tt>, as opposed to some other
/// <tt>Factory</tt> for a different abstract base class.
///
/// Every \link infact::Factory Factory\endlink-constructible
/// abstract type must also specify two methods, a
/// <tt>RegisterInitializers(Initializers&)</tt> method and an
/// <tt>Init(const string&)</tt> method. Both methods are guaranteed
/// to be invoked, in order, just after construction of every object
/// by the \link infact::Factory Factory\endlink.  To reduce the
/// burden on the programmer, you can derive your abstract class from
/// \link infact::FactoryConstructible FactoryConstructible
/// \endlink, which implements both methods to do nothing.  (All of
/// the abstract base classes that can be constructed via Factory in
/// the InFact Framework already do this.) For most concrete
/// subtypes, most of the work of initialization is done inside the
/// factory to initialize registered data members, handled by the
/// class&rsquo;s <tt>RegisterInitializers(Initializers&)</tt> method.
/// The implementation of this method generally contains a set of
/// invocations to the various <tt>Add</tt> methods of the \link
/// infact::Initializers Initializers \endlink class,
/// &ldquo;registering&rdquo; each variable with a name that will be
/// recognized by the \link infact::Factory Factory \endlink when it
/// parses the specification string. When member
/// initializations are added to an \link infact::Initializers
/// Initializers \endlink instance, they are optional by default.  By
/// including a third argument that is <tt>true</tt>, one may specify
/// a member whose initialization string <i>must</i> appear within the
/// specification.  If it does not contain it, a runtime error will be
/// raised.
///
/// For completeness, post&ndash;member-initialization may be
/// performed by the class&rsquo;s <tt>Init(const string &)</tt>
/// method, which is guaranteed to be invoked with the complete string
/// that was parsed by the \link infact::Factory Factory\endlink.
/// The code executed by a class&rsquo; <tt>Init(cosnt string &)</tt>
/// method is very much akin to the <i>declaration phase</i> of a
/// C++ constructor, because it is the code that gets executed just
/// after the members have been initialized.
///
/// For example, \link infact::Animal Animal
/// \endlink instances are \link infact::Factory
/// Factory\endlink-constructible, and so the \link
/// infact::Animal Animal \endlink class ensures
/// its concrete subclasses have a \link
/// infact::Animal::RegisterInitializers
/// RegisterInitializers \endlink method and an \link
/// infact::Animal::Init Init \endlink method by being a
/// subclass of \link infact::FactoryConstructible\endlink. As we
/// saw above, \link infact::Cow
/// Cow \endlink is a concrete subtype of \link
/// infact::Animal Animal\endlink .  That class
/// has two data members that can be initialized by a factory, one
/// required and one optional.  To show you how easy it is to
/// &ldquo;declare&rdquo; data members that need initialization, here
/// is the exact code from the \link
/// infact::Cow::RegisterInitializers
/// Cow::RegisterInitializers \endlink method:
/// \code
/// virtual void RegisterInitializers(Initializers &initializers) {
///   bool required = true;
///   initializers.Add("n",      &n_, required);
///   initializers.Add("prefix", &prefix_);
/// }
/// \endcode
/// The above code says that the \link infact::Cow
/// Cow \endlink has a data member <tt>n_</tt>,
/// which happens to be an <tt>int</tt>, that is required to be
/// initialized when an \link infact::Cow
/// Cow \endlink instance is constructed by a \link
/// infact::Factory Factory\endlink, and that the name of this
/// variable will be <tt>&quot;n&quot;</tt> as far as the factory is
/// concerned. It also says that it has a data member
/// <tt>prefix_</tt>, which happens to be of type <tt>string</tt>,
/// whose factory name will be <tt>&quot;prefix&quot;</tt>, and that
/// is not required to be present in a <i>specification string</i> for
/// an \link infact::Cow
/// Cow\endlink.
///
/// \subsection language The Factory language
///
/// As we&rsquo;ve seen, the language used to instantiate objects is
/// quite simple.  An object is constructed via a <i>specification
/// string</i> of the following form: \code
/// RegisteredClassName(member1(init1), member2(init2), ...) \endcode
/// where <tt>RegisteredClassName</tt> is the concrete subtype&rsquo;s
/// name specified with the \link REGISTER_NAMED \endlink macro (or,
/// more likely, one of the convenience macros that is
/// &ldquo;implemented&rdquo; in terms of the \link REGISTER_NAMED
/// \endlink macro, such as \link REGISTER_ANIMAL \endlink or \link
/// REGISTER_PET_OWNER\endlink).  The comma-separated list
/// inside the outermost set of parentheses is the set of <i>member
/// initializations</i>, which looks, as we saw \ref
/// cpp_example "above", intentionally similar to the format
/// of a C++ constructor&rsquo;s initialization phase.  The names of
/// class members that can be initialized are specified via repeated
/// invocations of the various overloaded \link infact::Initializers
/// \endlink <tt>Add</tt> methods. There is essentially one
/// <tt>Add</tt> method per primitive C++ type, as well as an
/// <tt>Add</tt> method for \link infact::Factory
/// Factory\endlink-constructible types.
///
/// If you love Backus-Naur Form specifications, please see the
/// documentation for the \link infact::Factory::CreateOrDie
/// Factory::CreateOrDie \endlink method for the formal description of
/// the grammar for specification strings.
///
/// To continue our example with \link infact::Cow
/// Cow \endlink, the following are all legal
/// specification strings for constructing \link
/// infact::Cow Cow \endlink
/// instances:
/// \code
/// Cow(name("brown"))
/// Cow(name("brown), age(2))
/// Cow(age(2), name("brown"))
/// Cow(name("brown"),)
/// \endcode
/// As you can see, the order of member initializers is not important
/// (because each has a unique name), and you can optionally put a
/// comma after the last initializer.  The following are
/// <i><b>illegal</b></i> specification strings for \link
/// infact::Cow Cow \endlink
/// instances:
/// \code
/// // Illegal specification strings:
/// Cow(age(4))
/// Cow()
/// Cow(name("fred"), age("brown"))
/// \endcode
/// In the first two cases, the specification strings are missing the required
/// variable <tt>name</tt>, and in the final case, the optional <tt>age</tt>
/// member is being initialized, but with an <tt>string</tt> literal instead of
/// a <tt>int</tt> literal.
///
///
/// \subsection putting_together Putting it all together
///
/// Here is a template illustrating how one creates a \link
/// infact::Factory Factory \endlink for an abstract base class
/// called &ldquo;<tt>Abby</tt>&rdquo; and declares a concrete subtype
/// &ldquo;<tt>Concky</tt>&rdquo; to that Factory.  Most users
/// of the InFact Framework are likely only to build concrete
/// subtypes of abstract classes that already have factories, and so
/// those users can safely ignore the <tt>abby.H</tt> and <tt>abby.C</tt>
/// files.
/// <ul>
///   <li> <tt>abby.H</tt>
/// \code
/// #include "factory.H"
/// class Abby : public FactoryConstructible {
///   // .. the code for Abby ...
/// };
/// #define REGISTER_NAMED_ABBY(TYPE,NAME) REGISTER_NAMED(TYPE,NAME,Abby)
/// #define REGISTER_ABBY(TYPE) REGISTER_NAMED_ABBY(TYPE,TYPE)
/// \endcode
///   <li> <tt>abby.C</tt>
/// \code
/// IMPLEMENT_FACTORY(Abby)
/// \endcode
///   <li> <tt>concky.H</tt>
/// \code
/// #include "abby.H"
/// class Concky : public Abby {
///  public:
///    virtual void RegisterInitializers(Initialiizers &initializers) {
///       // various calls to the overloaded Initializers::Add methods,
///       // one per data member that the Factory can initialize
///    }
/// };
/// \endcode
///   <li> <tt> concky.C </tt>
/// \code
/// REGISTER_ABBY(Concky)
/// \endcode
/// </ul>
///
/// So what about Greenspun&rsquo;s Tenth Rule?  Well, the idea that
/// initialization strings can themselves contain specification
/// strings suggests that there is a full-blown language being
/// interpreted here, complete with a proper tokenizer and a
/// recursive-descent parser.  There is.  It is a simple language, and
/// one that is formally specified.  To the extent that it mirrors the
/// way C++ does things, it is not quite <i>ad hoc</i>; rather, it is
/// (close to being) an exceedingly small subset of C++ that can be
/// executed dynamically.  We <i>hope</i> it is not bug-ridden, but
/// we&rsquo;ll let you, the user, be the judge of that.
