;; Вспомогательная: генерация всех способов размена суммы amount из stock
;; Возвращает список элементов вида (payout . new-stock), где
;; payout – список выданных номиналов (можно хранить как список для подсчёта длины)
(define (generate-payouts amount stock)
  (if (= amount 0)
      (list (cons '() stock))   ;; ничего не выдали, stock без изменений
      (if (null? stock)
          '()
          (let* ((head (car stock))
                 (d (car head))         ;; номинал
                 (q (cdr head))         ;; доступный запас
                 (rest-stock (cdr stock))
                 (max-use (min q (quotient amount d))))
            (append-map
             (lambda (k)
               (map
                (lambda (branch)
                  (let ((sub-payout (car branch))
                        (sub-stock  (cdr branch)))
                    ;; выдаём k купюр номинала d + то, что дала рекурсия
                    (cons (cons d (make-list k d)) ;; или сразу список из k элементов d
                          sub-stock)))
                (generate-payouts (- amount (* k d))
                                  (if (= k q)
                                      rest-stock
                                      (cons (cons d (- q k)) rest-stock)))))
             (range 0 max-use))))))

(define (solve stock agents)
  (if (null? agents)
      0
      (let* ((amount (car agents))
             (rest-agents (cdr agents))
             (branches (generate-payouts amount stock)))
        (apply min
               (map (lambda (branch)
                      (let* ((payout (car branch))
                             (new-stock (cdr branch))
                             (used (length payout)))
                        (+ used (solve new-stock rest-agents))))
                    branches)))))
