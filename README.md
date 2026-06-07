# Rekrutacja-TAG

Zadanie zaliczeniowe z WDP

Treść:
Masz dane n szklanek, ponumerowanych od 1 do n, o pojemnościach odpowiednio x1, x2, ..., xn.  Początkowo wszystkie szklanki są puste. Możesz wykonywać następujące czynności:
-nalać do wybranej szklanki do pełna wody z kranu, 
-wylać całą wodę z wybranej szklanki do zlewu, 
-przelać wodę z jednej szklanki do drugiej — jeżeli się zmieści, to przelewasz całą wodę, a jeżeli nie, to tyle żeby druga szklanka była pełna.

Twoim celem jest uzyskanie takiej sytuacji, że w każdej szklance jest określona ilość wody, odpowiednio y1, y2, ..., yn. 

Napisz program, który mając dane liczby x1, x2, ..., xn i y1, y2, ..., yn wyznaczy minimalną liczbę czynności potrzebnych do uzyskania opisanej przez nie sytuacji. Jeżeli jej uzyskanie nie jest możliwe, to poprawnym wynikiem jest -1. Dane należy wczytać ze standardowego wejścia w formacie:

n
x1 y1
x2 y2
...
xn yn

Możesz założyć, że 0 ≤ n oraz 0 ≤ yi ≤ xi dla i = 1, 2, ..., n. Wszystkie liczby na wejściu są całkowite. Liczby n oraz x1+x2+...+xn i y1+y2+...+yn mieszczą się w typie int.
