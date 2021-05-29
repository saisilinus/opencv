void thresholdSegmentation(char* inputImagePath, char* outputImagePath){
        Mat grad, threshed, blurred, kernel, closed;
        vector<vector<Point>> contours;
        vector<Point> largestContour;
        vector<Vec4i> hierarchy;
        RotatedRect rect;
        // vector<float> points(4,0);
        // vector<int> intPoints(4,0);
        Mat points = Mat(4, 2, CV_32F);
        Mat intPoints = Mat(4, 2, CV_32S);
        int ddepth = CV_32F;
        
        Mat input = imread(inputImagePath);

        Mat input_gray = imread(inputImagePath, IMREAD_GRAYSCALE);
        

        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;
        Sobel(input_gray, grad_x, ddepth, 1, 0, -1);
        Sobel(input_gray, grad_y, ddepth, 0, 1, -1);
       
        // Convert output to a CV_8U image
        convertScaleAbs(grad_x, abs_grad_x);
        convertScaleAbs(grad_y, abs_grad_y);

        //Gradient
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

        // blurring
        blur(grad, blurred, Size(9, 9));

        // Binary thresholding
        threshold(blurred, threshed, 230, 255, THRESH_BINARY);

        // get kernel for morphological operations
        kernel = getStructuringElement(MORPH_RECT, Size(21, 7));

        // closing morphological op
        morphologyEx( threshed, closed, MORPH_CLOSE, kernel );

        // find contours
        findContours(closed, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        //sort contours according to area
        sort(contours.begin(), contours.end(), compareContourAreas);

        // get largest contour
        largestContour = contours[0];

        rect = minAreaRect(largestContour);

        // find and draw points for rectangle
        // Point2f rectPoints[4];
        // rect.points(rectPoints);
        // Point2i intRectPoints[4];
        // intRectPoints = static_cast<Point>(rectPoints);

        // using boxPoints
        boxPoints(rect, points);
        // platform_log("Rotated Rect Points(FLOAT):/n bottomLeft: %d/n, topLeft: %d/n, topRight :%d/n, bottomRight: %dn", points[0], points[1], points[2], points[3]);
        // vector<int> intPoints(points.begin(), points.end());
        // points.convertTo(intPoints, CV_8S);
        // transform(points.begin(), points.end(), intPoints.begin(), convertToInt);
        // platform_log("Rotated Rect Points(INTEGER):/n bottomLeft: %d/n, topLeft: %d/n, topRight :%d/n, bottomRight: %dn", intPoints[0], intPoints[1], intPoints[2], intPoints[3]);
        // Mat pointsMat = Mat(4, 2, CV_32SC1);
        // pointsMat.push_back(intPoints);
        Mat pointsMat = Mat(1, 2, CV_32S);
        points.convertTo(intPoints, CV_32SC1);
        pointsMat.push_back(intPoints);
        
        drawContours(input, pointsMat, -1, Scalar(0, 255, 0), 3);

        imwrite(outputImagePath, input);
    }
