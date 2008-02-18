
class Test
	public string a, b, c;
	private real j, k;

	public Test()
	{
	}
end

class A
	inherit Test
	public integer c, d;

	public A(integer a, real d)
	{
		c := a * d;
	}
end

class MaClasse
    inherit A

    public MaClasse next;
    
    public MaClasse() {}

    public void main()
    {
	VAR MaClasse a;
	/*a := new MaClasse();*/
	a.maproc("salut", "test");
    }

    public MaClasse next()
    {
	return(next);
    }

    public integer ajout(integer a, integer b)
    {
	return(a+b);
    }

    private void maproc(string arg1, string arg2)
    {
        VAR integer a, b, c, d;

        if 1 = 3-2 then
            a := 5;
        else
        {
            VAR string s1, s2;
            b := 6;
            c := 7;

            while 1 do
                a := 2 + 7 * 8;


        }
        endif

        return ("salut");

    }
end
