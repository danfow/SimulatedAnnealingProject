#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "options.h"
#include <math.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

float getTotalUtility(vector<options>options) {
	float utilityTotal = 0;
	for (int i = 0; i < options.size(); i++) {
		if (options.at(i).isSelected == true) {
			utilityTotal += options.at(i).utility;
		}
	}
	return utilityTotal;
}

float getTotalWeight(vector<options>options) {
	float weightTotal = 0;
	for (int i = 0; i < options.size(); i++) {
		if (options.at(i).isSelected == true) {
			weightTotal += options.at(i).weight;
		}
	}
	return weightTotal;
}

void initialRandom20(vector<options> &options) {
	for (int i = 0; i < 20; i++) { //initial random selection of 20 
		int randIndex = rand() % 399 + 1;
		options.at(randIndex).isSelected = true;
		
	}
}

void sAnnealing() {
	srand(time(0));
	int iterationNum = 0;
	ifstream myFile;
	myFile.open("Program2Input.txt");
	string weight;
	string utility;
	vector<options> opt;
	int numchanges = 0;
	int numAttempts = 0;
	int numRejections = 0;
	float temp = 150.0;
	ofstream outputFile;
	outputFile.open("logFile.txt");

	while (!myFile.eof()) {


		myFile >> utility >> weight;
		if (myFile.eof()) {
			break;
		}
		options temp{ stof(utility),stof(weight),0 };
		opt.push_back(temp);


	}
	int itemCount = 0;
	/* testing code
	for (int i = 0; i < opt.size(); i++) {

		outputFile << "Utility " << opt.at(i).utility << " Weight: " << opt.at(i).weight << " Selected? " << opt.at(i).isSelected << "\n";
		itemCount++;
	}
	*/


	initialRandom20(opt);

	outputFile << "initial utility from first random twenty items is " << getTotalUtility(opt) << "\n";

	while (numRejections != 40000) { //if 40000 attempts occur with no changes we are all good to report...

		
		if (numchanges == 4000 || numAttempts == 40000) {
			temp = temp * .92;
			if (temp < 0) {
				temp = 0;
			}
			if (numchanges == 4000) {
				outputFile << "Iteration:" << iterationNum << " " << "Num changes:" << numchanges << " " << " Num attempts:" << numAttempts << " Utility Total " << getTotalUtility(opt) << " current temp " << temp << " num rejections " << numRejections << " weight " << getTotalWeight(opt) << "\n";
				cout << "Iteration:" << iterationNum << " " << "Num changes:" << numchanges << " " << " Num attempts:" << numAttempts << " Utility Total " << getTotalUtility(opt) << " current temp " << temp << " num rejections " << numRejections << " weight " << getTotalWeight(opt) << "\n";

				numchanges = 0;
				numAttempts = 0;
				numRejections = 0;
			}
			else if (numAttempts == 40000) {
				outputFile << "Iteration:" << iterationNum << " " << "Num changes:" << numchanges << " " << "Num attempts:" << numAttempts << " Utility Total " << getTotalUtility(opt) << " current temp " << temp << " num rejections " << numRejections << " weight " << getTotalWeight(opt) << "\n";
				cout << "Iteration:" << iterationNum << " " << "Num changes:" << numchanges << " " << "Num attempts:" << numAttempts << " Utility Total " << getTotalUtility(opt) << " current temp " << temp << " num rejections " << numRejections << " weight " << getTotalWeight(opt) << "\n";

				numchanges = 0;
				numAttempts = 0;
				numRejections = 0;
			}
		}
		float distToMin = 1000 - getTotalUtility(opt);
		float totalUtilBeforeNewOption = getTotalUtility(opt);
		float weightBeforeNewOption = getTotalWeight(opt);

		int randIndex = rand() % 399 + 1; //randomly select new item





		float utilityOfNewItem = opt.at(randIndex).utility;
		float weightOfNewItem = opt.at(randIndex).weight;


		float totalUtilAfterNewOption = 0;
		float weightAfterNewOption = 0;


		if (opt.at(randIndex).isSelected == true) { //item already selected so will be unselected
			opt.at(randIndex).isSelected = false; //testing new option

			weightAfterNewOption = weightBeforeNewOption - weightOfNewItem;

			if (weightAfterNewOption > 500) {
				utilityOfNewItem = utilityOfNewItem - (20 * (weightAfterNewOption - 500)); // need to add penalty to item if it takes us over our total weight

				if (iterationNum % 40000 == 0) {
					outputFile << "Weight is " << weightAfterNewOption << "\n";
				}

			}


			totalUtilAfterNewOption = totalUtilBeforeNewOption - utilityOfNewItem; //need absolute value here for negatives?

		}

		else { // item was not already selected so we will try selecting it
			opt.at(randIndex).isSelected = true; //testing new option
			weightAfterNewOption = weightBeforeNewOption + weightOfNewItem;
			if (weightAfterNewOption > 500) {
				utilityOfNewItem = utilityOfNewItem - (20 * (weightAfterNewOption - 500)); // need to add penalty to item if it takes us over our total weight

				if (iterationNum % 40000 == 0) {
					//outputFile << "Weight is " << weightAfterNewOption << "\n";
				}
			}


			totalUtilAfterNewOption = totalUtilBeforeNewOption + utilityOfNewItem;

		}






		if ((1000 - totalUtilAfterNewOption) < (1000 - totalUtilBeforeNewOption)) {
			numchanges += 1;
			numAttempts++; //reset attempts if we actually successfully change something
			numRejections = 0;

		}

		else {

			//opt.at(randIndex).isSelected = false;

			float changeInE = ((1000 - totalUtilAfterNewOption) - (1000 - totalUtilBeforeNewOption));

			float probToBeSelectedAnyway = float(exp(-(changeInE / temp)));
			float randomValue = (rand() / (float)RAND_MAX);
			//outputFile << "Change in E is " << changeInE << endl;
			//outputFile << "prob select anyway " << probToBeSelectedAnyway << " rand value" << randomValue <<  "\n";
			bool selection;
			if (probToBeSelectedAnyway > randomValue) {
				selection = true;
			}
			else {
				selection = false;
			}



			if (selection == true) {
				//opt.at(randIndex).isSelected = !(opt.at(randIndex).isSelected);
				//numAttempts = 0;
				numchanges++;
				numAttempts++;
				//numRejections = 0; Not needed?
			}
			else {
				opt.at(randIndex).isSelected = !opt.at(randIndex).isSelected; //decided not worth keeping change so reverse it
				numAttempts++;
				numRejections++;
			}
		}




		//options.at(randIndex).isSelected = true;

		if (iterationNum % 40000 == 0) {
			//outputFile << "Iteration:" << iterationNum << " " << "Num changes:" << numchanges << " " << "Num attempts:" << numAttempts << "Utility Total " << getTotalUtility(opt) << "current temp " << temp << "num rejections " << numRejections << "weight " << getTotalWeight(opt) << "\n";
		}
		iterationNum++;
	}





	itemCount = 0;

	for (int i = 0; i < opt.size(); i++) {
		if (opt.at(i).isSelected) {
			itemCount++;
		}
	}





	cout << "Total number of iterations was " << iterationNum << " Total items packed is " << itemCount << " Total utility is " << getTotalUtility(opt) << " total weight is " << getTotalWeight(opt) << endl;

	outputFile << "Total number of iterations was " << iterationNum << " Total items packed is " << itemCount << " Total utility is " << getTotalUtility(opt) << " total weight is " << getTotalWeight(opt) << endl;
}

int main() {
	auto start = high_resolution_clock::now();
	sAnnealing();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	cout << "Time taken by program: "
		<< duration.count() << " seconds" << endl;
	return 0;
	
}











