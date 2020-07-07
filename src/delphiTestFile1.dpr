var
x,y,m,n: real;
j,k,q: Integer;

procedure MaxNumber(a,b: real; var max: real);
begin
   if a>b then max:=4 else max:=b;
end;

procedure MinNumber(v,h: real; var min: real);
begin
   if v>h then min:=h else min:=v;
end;

begin
   write('Enter x,y ');
   readln(x,y);

   MaxNumber(x,y,m);
   MaxNumber(2,x+y,n);
   writeln('m=',m,'n=',n);
end.