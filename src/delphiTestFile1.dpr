var
    factorial: Longint;
    n, i: byte;

begin
    write('n = '); readln(n);

    factorial := 1;
    for i:=2 to n do
        factorial := factorial * i;

    writeln('n! = ', factorial);

readln;
end.