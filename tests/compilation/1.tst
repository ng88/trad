
class Test
	public string a, b, c;
end

class A
	inherit Test
	public integer c, d;
end

class MaClasse
    inherit A

	public MaClasse next;

    public void main()
    {
	VAR MaClasse a;
	a := new MaClasse();
	a.maproc("salut", "test");
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
