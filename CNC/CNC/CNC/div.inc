;
;    Atmel AVR Math Library   - multiplication functions definitions
;
;    Copyright (C) 2013  Lucian Hanga
;
;    This program is free software: you can redistribute it and/or modify
;    it under the terms of the GNU General Public License as published by
;    the Free Software Foundation, either version 3 of the License, or
;    (at your option) any later version.
;
;    This program is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
;
.macro div8
                ;
                ; proc Div8U    - (Divide 8bit Unsigned)
                ;       descr:  8bit unsigned integer division
                ;       params: r16 - Nominator (N)
                ;               r17 - Denominator (D)
                ;       ret:    r18 - Quotinent (Q)
                ;               r19 - Rest (R) 
                ;       alters: r18, r19
Div8U:          push            r20                             ; save the registers which will be altered
                push            r21                             ; r20, r21, r22
                push            r22                             ; --
                in              r22, SREG                       ; save the Status Register
                push            r22                             ; --
                .def            N = r16
                .def            D = r17
                .def            Q = r18
                .def            R = r19
                .def            index = r20
                .def            tmp = r21
                .def            tmpN = r22
                ldi             index, 0x08                     ; load the index the bit size
                mov             tmpN, N                         ; tmpN <-- N
                clr             Q                               ; Q <-- 0
                clr             R                               ; R <-- 0
Div8U_E10:                                                      ; do
                lsl             tmpN                            ;   Shift Left R:N - get in R bit by bit N, starting
                rol             R                               ;                      from the most significant bit
                lsl             Q                               ;   Q <-- Q << 2
                mov             tmp, R                          ;   tmp <-- R - D
                sub             tmp, D                          ;   --
                brlo            Div8U_E20                       ;   if (R > D) then   [IMPORTANT: UNsinged comparation]
                mov             R,tmp                           ;       R <-- R - D
                inc             Q                               ;       Q <-- Q + 1 
Div8U_E20:      dec             index                           ;   index <-- index - 1  
                cpi             index, 0x00                     ; while (index > 0)
                brne            Div8U_E10                       ; --
                pop             r22                             ; restore the Status Register
                out             SREG, r22                       ; --
                pop             r22                             ; restore the altered registers
                pop             r21                             ; r22, r21, r20
                pop             r20                             ;
                .undef          N
                .undef          D
                .undef          Q
                .undef          R
                .undef          index
                .undef          tmp
                .undef          tmpN
.endm
