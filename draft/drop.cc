// extern void write_result_to_txt(const char *ofname, int maxrow, const std::valarray<double _Complex> &valdata, bool judge_4pt);


// void write_result_to_txt(const char *ofname, int maxline, const VARRAY_COMPLEX &valdata, bool judge_4pt)
// {
//   fprintf(stderr, "generating %s\n", ofname); // print the output file name (print to the stderr file)

//   FILE *ofp = NULL;
//   ofp = fopen(ofname, "w");
//   if (ofp == NULL)
//   {
//     perror(ofname);
//     exit(1);
//   }

//   if (judge_4pt)
//   {
//     int max_distance = 0;

//     for (int i = 0; i < maxline / 2 + 1; i++)
//       for (int j = i; j < maxline / 2 + 1; j++)
//         for (int k = j; k < maxline / 2 + 1; k++)
//         {
//           max_distance++;
//         }

//     double distance[max_distance];
//     int index = 0;

//     for (int i = 0; i < maxline / 2 + 1; i++)
//       for (int j = i; j < maxline / 2 + 1; j++)
//         for (int k = j; k < maxline / 2 + 1; k++)
//         {
//           double temp = i * i + j * j + k * k;
//           temp = pow(temp, 0.5);
//           distance[index] = temp;
//           ;

//           index++;
//         }

//     for (int i = 0; i < max_distance; i++)
//     {
//       fprintf(ofp, "%1.16e  %1.16e  %1.16e\n", distance[i], __real__ valdata[i], __imag__ valdata[i]);
//     }
//   }
//   else
//   {
//     for (int i = 0; i < maxline; i++)
//     {
//       fprintf(ofp, "%d  %1.16e  %1.16e\n", i, __real__ valdata[i], __imag__ valdata[i]);
//     }
//   }

//   fclose(ofp);
// }