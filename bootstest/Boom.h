#ifndef _BOOM_H_
#define _BOOM_H_

class Boom : public Animation {
public:
	Boom(MultiNeoPixel& strip, bool enabled) : Animation(strip, enabled) {
		mCX = mCY = 0;
		mRadius = 0.0f;
		mSpeed = 10.0f;
		mMaxRadius = 100.0f;
		mInProgress = false;
		mChanceOf1000 = 100;
	}

	void setChanceOf1000(uint16_t chance) {
		mChanceOf1000 = chance;
	}

	virtual void begin() {
		clear();
	}

	virtual void clear() {
		mInProgress = false;
		mRadius = 0.0f;
	}

	bool shouldExpand() {
		/*
		uint16_t chance = random(0, 1000);
		return (chance < mChanceOf1000);
		*/
		return (getFrameCount() % 64) == 0;
	}

	void randomCenter() {
		mCX = randomX();
		mCY = randomY();
	}

	void expand() {
		mInProgress = true;

		mRadius += mSpeed;
		int8_t dx = (int8_t)((int16_t)mRadius / (int16_t)SPACE_X_MM);
		int8_t dy = (int8_t)((int16_t)mRadius / (int16_t)SPACE_Y_MM);
		int8_t xmin = mStrip.clampX(mCX - dx);
		int8_t xmax = mStrip.clampX(mCX + dx);
		int8_t ymin = mStrip.clampY(mCY - dy);
		int8_t ymax = mStrip.clampY(mCY + dy);

		Serial.print("r=");
		Serial.print(mRadius);
		Serial.print(", dx=");
		Serial.print(dx);
		Serial.print(", dy=");
		Serial.print(dy);
		Serial.print(", xmin=");
		Serial.print(xmin);
		Serial.print(", ymin=");
		Serial.print(ymin);
		Serial.print(", xmax=");
		Serial.print(xmax);
		Serial.print(", ymax=");
		Serial.print(ymax);
		Serial.println();

		for (int8_t x = xmin; x <= xmax; x++) {
			for (int8_t y = ymin; y <= ymax; y++) {
				float xsqr = (float)abs(mCX - x) * (float)SPACE_X_MM;
				float ysqr = (float)abs(mCY - y) * (float)SPACE_Y_MM;
				float distance = sqrt(xsqr*xsqr + ysqr*ysqr);
				if (distance <= mRadius) {
					mStrip.setPixelColor(x, y, 255, 255, 255);
				}
			}
		}

		if (mRadius >= mMaxRadius) {
			mInProgress = false;
		}
	}


	void beginExpand() {
		mCX = randomX();
		mCY = randomY();
		mRadius = 0.0f;
		mInProgress = true;

		Serial.print("begin: mr=");
		Serial.print(mMaxRadius);
		Serial.print(", ");
		Serial.print(mStrip.getSizeX());
		Serial.print(", ");
		Serial.println(mStrip.getSizeY());
	}

	int8_t randomX() {
		return random(1, mStrip.getSizeX()-1);
	}

	int8_t randomY() {
		return random(1, mStrip.getSizeY()-1);
	}

protected:
	virtual void performDraw() {
		if (mInProgress) {
			expand();
		} else {
			if (shouldExpand()) {
				beginExpand();
			}
		}
	}

private:
	int8_t mCX;
	int8_t mCY;
	float mRadius;
	float mMaxRadius;
	float mSpeed;
	uint16_t mChanceOf1000;
	bool mInProgress;
};

#endif // #ifndef _BOOM_H_
