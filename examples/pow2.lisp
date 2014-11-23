(defun <= (x y) (or (< x y) (= x y)))

(defun range (x y)
	(if (<= x y)
		(cons x (range (+ x 1) y))))

(defun each (lst fn)
	(if lst
		(progn
			(fn (car lst))
			(each (cdr lst) fn))))

(defun map (lst fn)
	(if lst
		(cons (fn (car lst))
			(map (cdr lst) fn))))

(defun pow2 (n) (* n n))

(define to_print (map (range 0 10) pow2))

(println '"Nadruhou ...")
(each to_print (lambda (n) (progn 
	(println '--------)
	(println n))))
