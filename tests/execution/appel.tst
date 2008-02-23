
class MaClasse

public MaClasse() {}

    public void main()
    {
	VAR integer i;

	i := 0;

	while i < 10 do
	{
		prints("fact(");
		printi(i);
		prints(")=");
		printi(fact(i));
		prints("\n");
		i := i + 1;
	}

    }

  public integer fact(integer i)
  {
	if i <= 1 then
	    return(1);
        else
	    return(fact(i - 1) * i);
	endif
  }

end
