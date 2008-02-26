
class A

	private integer val;

	public A(integer e) { val := e; }

	public A add(integer e)
	{
		VAR A a;
		a := new A(val + e);
		return (a);
	}
	
	public void printVal() { printi(val); prints("\n"); }

end

class B

	public B() {}

	public A test1(integer v, integer b)
	{
		VAR A a;
		a := new A(v * b);
		return (a);
	}

end


class MaClasse

public MaClasse() {}

    public void main()
    {
	VAR B b;

	b := new B();
	b.test1(5, 6).add(2).add(7).printVal();


    }


end
