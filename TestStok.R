#  Test -  find stochastic period (from note)
par(pty = 's')
plot(c(0,1),c(0,1), t='n', asp=1, xlab='x_n',ylab  ='x_{n+1}')
numits = 10500

P = 3 # period to look for: don't use "T" because that means "True"
N= 7
#gam = 0.066
gam = 0
#noise = 0
#noise   = .01
noise =  .13
base =  4
#base = 3.88
#base = 3.3

#set.seed(47)
x=runif(1)
#x = .2371
orbit = c()
for (i in 1:(N*P)){
  xnext= (base-noise + noise*runif(1))*x*(1-x)
  orbit  = c(orbit,xnext)
  x = xnext
}

# from the "Finding Cycles in nonlinear dynamics" paper
###############################################
## for T=3, N=5
# aj1 =  13/14
# aj2 = 10/7*(cos(3*pi/14)-sin(pi/7))
# aj3=1-cos(pi/7)+sin(pi/14)
# aj4=4/7*(cos(3*pi/14)-sin(pi/7))
# aj5 = 1/14
# ajs=c(aj1,aj2,aj3,aj4,aj5)
#  from the "Finding Cycles"  paper, but calculating from definitions
#  This gives the same values even though  they  look different: it was just a sanity check
#### for T=3, N=5
# aj1 = 13/14
# aj2 = -10/7*(cos(5*pi/14)+cos(11*pi/14))
# aj3 = cos(8*pi/7)+cos(3*pi/7)+1
# aj4 = -4/7*(cos(5*pi/14)+cos(11*pi/14))
# aj5 =  1/14
# ajs=c(aj1,aj2,aj3,aj4,aj5)
#############################################
## for T=3, N=6
# t1 =5*pi/17
# t2=11*pi/17
# c1=1
# c2=1-2*(cos(t1)+cos(t2))
# c3=2*(1+2*cos(t1)*cos(t2)-(cos(t1)+cos(t2)))
# c4=c3
# c5=c2
# c6=1
# cs=c(c1,c2,c3,c4,c5,c6)
#############################
# ## for T=3, N=7
# # # (sigma, tau)=(2,2) gives fixed point, (2,3) gives period 2
# # # curious bifurcations occur as tau increases, e.g. (2,3.6)
sigma = 2
tau = 3.7
tjs=c()
for (j in 1:((N-1)/2)){
  tjs[j]=pi*(sigma + P*(2*j-1))/(tau + (N-1)*P)
}
t1=tjs[1]
t2=tjs[2]
t3=tjs[3]
B1=-2*(cos(t1)+cos(t2))
B2 = 2*(1+2*cos(t1)*cos(t2))
c1=1
c2=B1-2*cos(t3)
c3 = 1-2*cos(t3)*B1+B2
c4=2*(B1-cos(t3)*B2)
c5=c3
c6=c2
c7=1
cs=c(c1,c2,c3,c4,c5,c6,c7)
##################################
###  T=8, N=12
# sigma = 2
# tau = 2
# tjs=c()
# b=c()
# for (j in 1:((N-2)/2)){
#   tjs[j]=pi*(sigma + P*(2*j-1))/(tau + (N-1)*P)
#   b[j]=2*cos(tjs[j])
# }
# k10 = 1
# k9 = sum(b)
# k8 = 1+b[5]*sum(b[1:4]) + 4+b[1]*b[2]+b[3]*b[4]+(b[1]+b[2])*(b[3]+b[4])
# k7 =-(2*sum(b[1:4])+b[5]*(4+b[1]*b[2]+b[3]*b[4]+(b[1]+b[2])*(b[3]+b[4]))
#       +(b[1]+b[2])*(2+b[3]*b[4])  + (b[2]+b[3])*(2+b[1]*b[2]) )
# k6 = 4 + b[1]*b[2]+b[3]*b[4]+(b[1]+b[2])*(b[3]+b[4])
#      +b[5]*(sum(b[1:4]) + (b[1]+b[2])*(2+b[3]*b[4])+(b[3]+b[4])*(2+b[1]*b[2]))
#      + 2 + 2*(b[1]+b[2])*(b[3]+b[4])+(2+b[1]*b[2])*(2+b[3]*b[4])
# k5 = -(2*(sum(b[1:4])+(b[1]+b[2])*(2+b[3]*b[4])+(b[3]+b[4])*(2+b[1]*b[2]))
#        +b[5]*(2 + 2*(b[1]+b[2])*(b[3]+b[4])+(2+b[1]*b[2])*(2+b[3]*b[4]))
#        )
# k4=k6
# k3=k7
# k2=k8
# k1=k9
# k0=k10
# ks=c(k0,k1,k2,k3,k4,k5,k6,k7,k8,k9,k10)
# cs=ks[1:10]+ks[2:11]
# cs=c(1,cs,1)  # vector of length 12
#############################################
##### MAKE the actual coefficients ##########
#############################################
ajs=c()
for(j in 1:N){
  ajs[j]=(1-(1+(j-1)*P)/(2+(N-1)*P))*cs[j]
}

# Normalize so that the  ajs sum to one
K=sum(ajs)
ajs = ajs/K

# for (j in 1:N){
  # Fejer coefficients
  #ajs[j]=(2/N)*(1  -  (2*j-1)/(2*N))
  # From arxiv paper
  # ajs[j]= (2*(N-j)+1)/(N^2)
  # Dirichlet coefficients
  # ajs[j]=1/N
#}

# # From "Cycle" paper
# eps2 = 1/9
# eps1 = 4/9
# ajs[3]=eps2
# ajs[2]  = eps1-eps2
# ajs[1] = 1-(ajs[2]+ajs[3])

## NOTE: we need to reverse the order of the weight vector 
## because we put the averaged vector in increasing index order
##  Do not reverse in T=8, N=12 case
ajs = rev(ajs)

# try starting with random orbit values
#orbit = runif(N*P,0,1)

kvals = c()
for (k in (N*P):(numits+N*P)){
  #kvals  = c(kvals,k)
  S1 = seq((k-(N-1)*P),k,by = P) # which previous points to average over, part 1
  S2 = seq((k -(N*P)+1),(k-P+1),by = P)  # which points to average, part 2
  xnext = (1-gam)*sum(ajs*(base-noise + noise*runif(N))*orbit[S1]*(1-orbit[S1]))
          + gam*(1/N)*sum((base-noise + noise*runif(N))*orbit[S2]*(1-orbit[S2]))
   orbit = c(orbit,xnext)       
}
range = (numits-2000):(numits-1)

points(orbit[range],orbit[(range+1)],xlim=c(0,1),ylim=c(0,1), col=
         'red', cex=.1+N/30)
lines(c(0,1),c(0,1),col = 'green')
# plot a graph of the Pth iterate of the deterministic map on top
a1 = base-noise
a2 = base

x0vals  = seq(0,1,by = .01)
x1vals = x0vals
x2vals = x0vals
for (j in 1:P){
 x1vals = a1*x1vals*(1-x1vals)
 x2vals = a2*x2vals*(1-x2vals)
}
lines(x0vals,x1vals,col = 'purple')
lines(x0vals,x2vals,col = 'lavender')
lines(x0vals,a2*x0vals*(1-x0vals),col = 'blue')

# calculate period 2 orbit
h   = 4
p1 =  (1+h -   sqrt(h^2-2*h-3))/(2*h)
p2 =  (1+h +   sqrt(h^2-2*h-3))/(2*h)
# plot on  top  of graph# 
# points(c(p1,p2),c(p2,p1),pch = 16,col='red')

# for the time series plot
range_ts = 10000:10300
par(pty =  'm')
plot(range_ts,orbit[range_ts],type ='b',pch=20)
#plot(range,orbit[range],type ='l')

# look at  the distribution of orbit points
hist(orbit[range],breaks = 100, freq = F)
#  plot period 2 points on horizontal axis
# points(c(p1,p2),c(0,0),pch=16,col='red')

## TO LISTEN!!  ###############
#install.packages("audio")
#library('audio')
o2=orbit-mean(orbit)
#play(o2,1000)

