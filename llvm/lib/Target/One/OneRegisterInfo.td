class OneReg<bits<5> Enc, string n, list<string> alt = []> :Register<n>{
    let HWEncoding{4-0} = Enc;
    let Namespace = "One";
    let AltNames = alt;
}

 

def ABIRegAltName : RegAltNameIndex; 

let RegAltNameIndices = [ABIRegAltName] in {
    let isConstant = true in {
        def X0 : OneReg<0, "x0", ["zero"]>;
    }

    def X1  :   OneReg <1,  "x1" ,   ["ra"]>;
    def X2  :   OneReg <2,  "x2" ,   ["sp"]>;
    def X10 :   OneReg <10, "x10" ,  ["a0"]>;
    def X11 :   OneReg <11, "x11" ,  ["a1"]>;
}


def GPR : RegisterClass<"One", [i32], 32, (add X0, X1, X2, X10, X11)>;





