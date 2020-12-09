# A personal financial planning program

This personal financial planning program is a system to manage the capital for clients who are planning for their retirement. For this purpose, this program only invests in very low risk products like fictional interest-bearing products, which mean we ignore the risk of product default. In order to show the performance in capital management, a trading simulation program is supply to show the transaction records of a certain group of clients and products. Notes that the program is designed to simulate trading decision (algorithm) for every client in every single day from 5th April 2020 to each client retirement date.

## A summary of the algorithm

<div align=center><img src="https://github.com/zhou9539/hello-world/blob/main/PFPP%20-%20Flow%20Chart.png"/></div>

## Algorithm efficiency

Here is algorithm efficiency is not about the code running time, but the overall returns. 

When I started thinking about algorithms, the first thing I thought of was that it could be regarded as a linear programming problem. However, because the products invested are low-risk, it is difficult for us to find suitable constraints for linear programming (only the maximum investment amount and the minimum investment amount can be set). Then what it does is find the product with highest rate. In addition, linear programming itself cannot deal with the product's tie-in period and client’s retirement date, which are crucial factor.

I chose to use the most primitive method (to invest in the product with the highest rate of return), with some modification based on tie-in period, anniversary and retirement date. It is similar to greedy algorithm, as we only maximize the immediate benefit. This algorithm is far from perfect, but at least it is a feasible one. 

There should be an algorithm that perfectly connects all investment period, but cannot find it, so I try to improve the primitive one. During the tie-in period, of course, the product cannot be traded, but once this period has passed, we should immediately consider whether to replace it with a better product. 

If there are better products (higher interest rate, shorter tie-in period), it is acceptable to lose some interest after last anniversary, but do not want to lose too much. We could set a certain number of days after the anniversary, within this period, we can sell the products for better one. If the previous anniversary has passed for a long time, we chose to continue to hold.

Another modification is based on retirement date. If the customer is about to retire soon, the tie-in period is very important, so we choose products with a higher interest rate within the acceptable range of the tie-in period.

According the performance of trading simulation of the example Feed.csv, the modified algorithm did better (the overall capital of all clients slightly larger).
