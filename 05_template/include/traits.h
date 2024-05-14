// When Ts   is declared, it must be prefixed by ellipsis in form : typename...Ts
// When Ts   is expanded, it must be followed by ellipsis in form : (pattern Ts)...
// When args is declared, it must be prefixed by ellipsis in form : (pattern Ts)...args <--- case 3
// When args is expanded, it must be followed by ellipsis in form : (pattern args)...
// 
// which means : 
// (pattern T0), (pattern T1), (pattern T2) ...
// (pattern arg0), (pattern arg1), (pattern arg2) ...
//
// Remark : 
// Case 3 is expansion of Ts as well we declaration of args.
// Case 3 has ellipsis lying between (pattern Ts) and args.
//
// Expressions have args twice, needs to be expanded twice,
// may be expanded in 2 dimensions, depending on pattern applied.
//



