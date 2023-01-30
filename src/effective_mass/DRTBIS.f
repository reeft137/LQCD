        FUNCTION rtbis(func,x1,x2,xacc)
        INTEGER JMAX
        DOUBLE PRECISION rtbis,x1,x2,xacc,func
        EXTERNAL func
        PARAMETER (JMAX=4000)
        INTEGER j
        DOUBLE PRECISION dx,f,fmid,xmid
        fmid=func(x2)
        f=func(x1)
        if(f*fmid.ge.0.d0)then
        write(*, *)'root must be bracketed in rtbis'
        read(*, *)
        endif
        if(f.lt.0.d0)then
        rtbis=x1
        dx=x2-x1
        else
        rtbis=x2
        dx=x1-x2
        endif
        do 11 j=1,JMAX
        dx=dx*.5d0
        xmid=rtbis+dx
        fmid=func(xmid)
        if(fmid.le.0.d0)rtbis=xmid
        if(abs(dx).lt.xacc .or. fmid.eq.0.d0) return
11      continue
        write(*, *)'too many bisections in rtbis'
        read(*, *)
        END
