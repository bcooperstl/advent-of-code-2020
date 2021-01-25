Manipulation Lookup Table:

```
                                                TO:
                    NorthFront  EastFront   SouthFront  WestFront   NorthBack   EastBack    SouthBack   WestBack
        NorthFront  Rotate0     Rotate90    Rotate180   Rotate270   VertFlip    /Flip       HorzFlip    \Flip
        EastFront   Rotate270   Rotate0     Rotate90    Rotate180   /Flip       HorzFlip    \Flip       VertFlip
        SouthFront  Rotate180   Rotate270   Rotate0     Rotate90    HorzFlip    \Flip       VertSlip    /Flip
        WestFront   Rotate90    Rotate0     Rotate270   Rotate0     \Flip       VertFlip    /Flip       HorzFlip
FROM:   NorthBack   VertFlip    /Flip       HorzFlip    \Flip       Rotate0     Rotate90    Rotate180   Rotate270   
        EastBack    /Flip       HorzFlip    \Flip       VertFlip    Rotate270   Rotate0     Rotate90    Rotate180   
        SouthBack   HorzFlip    \Flip       VertFlip    /Flip       Rotate180   Rotate270   Rotate0     Rotate90    
        WestBack    \Flip       VertFlip    /Flip       HorzFlip    Rotate90    Rotate0     Rotate270   Rotate0     
```