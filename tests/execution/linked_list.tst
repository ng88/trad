



/* Exemple de programme : linked list d'entier avec iterator
*/


/** Un noeud de la LinkedList */
class Node

    private integer val;
    private Node next;

    public Node(integer v)
    {
	val := v;
	next := nil;
    }

    public integer val() { return(val); }
    public Node next() { return(next); }

    public void setVal(integer v) { val := v; }
    public void setNext(Node n) { next := n; }

end

/** Iterator (interface generale) */
class Iterator
     public Iterator() {}
     public integer hasNext() { return (0); }
     public integer next() { return (0); }
end

/** Iterator pour notre LinkedList */
class LLIterator
	inherit Iterator

    private Node current;

    public LLIterator(Node start) { current := start; }
    public integer hasNext() { return(current != nil); }
    public integer next()
    {
	VAR integer val;

	if current = nil then
		return(0);
	endif
	
	val := current.val();
	current := current.next();
	return(val);
    }
end

/** Linked list */
class LinkedList

    private Node first;
    private Node last;
    private integer size;

    public LinkedList()
    {
	first := nil;
	last := nil;
	size := 0;
    }

   public void add(integer v)
   {
	VAR Node n;
	n := new Node(v);

	if first = nil then
	    first := n;
	else
		last.setNext(n);
	endif

	last := n;
	size := size + 1;
   }


   public Iterator iterator()
   {
	VAR Iterator ret;
	ret := new LLIterator(first);
	return(ret);
   }


   public void clear()
   {
	VAR Node current, t;
	current := first;
	while current != nil do
	{
		VAR string foo;
		t := current.next();
		freeobj(current);
		current := t;
	}
	size := 0;
	first := nil;
	last := nil;
   }

   public integer size() { return(size); }

end


class TestLL

    public TestLL() {}

    public void main()
    {
	VAR LinkedList l1;
	VAR Iterator it;

	l1 := new LinkedList();

	l1.add(3);
	l1.add(36);
	l1.add(89);
	l1.add(0);
	l1.add(5);
	l1.add(-623);
	l1.add(12);

	prints("Taille de la liste=");
	printi(l1.size());
	prints("\n");

	it := l1.iterator();
	while it.hasNext() do
	{
		prints("valeur=");
		printi(it.next());
		prints("\n");
	}

	l1.clear();
	freeobj(l1);

    }

end


