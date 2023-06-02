#include<iostream>
#include"Functions.h"
int main () {
   int i, j, N;
   double s_start, s, T, r, sigma, sigma2,  sigma2_start, mu, alpha, beta,
          gamma, dt, Z, R, U, sigma2_LT;
   double s_til,s_anti,s_bar,s_2bar,callPrice, s_final;
   double K = 0.0;
   double n =0.0, discount;
   double C,Ctil;
   double S0,C0, C0til,X,C0bar = 0.0 ,C02bar =0.0;
   double stdhatC,error, elapsed_time, t_star, epsilon;
   int counter, test , done;

   // Specify the alpha, beta, and gamma parameters.
   j = GetInteger ("Which problem (2, 3 ,or 4) are you working on?... ");
   if (j == 2) {
      alpha = 0.0; beta = 0.0; gamma = 1.0;
   }
   else if (j== 3) {
      alpha = 0.01; beta = 0.0; gamma = 0.99;
   }
   else {
      alpha = 0.01; beta = 0.10; gamma = 0.89;
   }
   // Time to expiration.
   
   T = 0.5;

   // Risk-free interest rate.
   r = 0.05;

   // Discount factor
   discount = exp(-r*T);

   // Number of days to expiration (252 days per year), and one day in years.
   N = 252 * T;
   dt = 1.0/252.0;

   // Convert "r" to a daily risk-free rate.
   r *= dt;

   // Current stock price.
   s_start = 100.0;

   // Current stock price variance.
   sigma2_start = 0.35 * 0.35;

   // Current daily stock price variance.
   sigma2_start *= dt;

   // Annual long-term variance.
   sigma2_LT = 0.30 * 0.30;

   // Daily long-term variance.
   sigma2_LT *= dt;

   // Seed the RNG.
   MTUniform (1);

   Time();
   // Generate and display 5 illustrative paths.
   
   //}
   for (j = 1; j <= 1000000; j++) {

      // Initialize the stock price and the volatility to their starting (time 0)
      //    values.
      s = s_start;
      s_til = s_start;
      sigma2 = sigma2_start;

      // Now march forward in time day by day.
      for (i = 1; i <= N; i++) {

         // Compute the drift parameter that corresponds to the volatility for
         //    this period.
         mu = r - 0.5*sigma2;

         // Compute the stock price at day "i"...

         // First get a standard normal Z.
         U = MTUniform (0);
         Z = PsiInv (U);

         // Apply current volatility.
         sigma = sqrt(sigma2);
         R = sigma * Z;

         // Update the stock price.
         s *= exp (mu + R);

         s_til *= exp (mu - R);

         // Update the stochastic volatility according to the GARCH(1,1) model.
         sigma2 = alpha * sigma2_LT  +  beta * R*R +  gamma * sigma2;

         if(i==N){
            if(s - K > 0){C = s - K;}
               else{C = 0;}
            if(s_til - K > 0){Ctil = s_til - K;}
               else{Ctil = 0;}
         }
      }
      
      // Discounting the call price to time 0, and calculating the antithetic call price
      C0 = discount * C;
      C0til = discount * Ctil;
      X = (C0 + C0til) / 2.0;

      // Computing sample moments 
      C0bar  = ((j-1)*C0bar + X) / j;
      C02bar  = ((j-1)*C02bar + X*X) / j;
      
      // Computing the error, and displaying results
      error = 1.96 * sqrt((C02bar-C0bar*C0bar)/1000000.0);
      printf ("Expected present value is %6.2f +/- %6.2f ", C0bar, error);
      printf ("with 95%% confidence.\n");
      
   } 
   Exit();
}
