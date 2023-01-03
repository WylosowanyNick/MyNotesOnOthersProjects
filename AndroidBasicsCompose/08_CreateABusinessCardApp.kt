package com.example.businesscard

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.*
import androidx.compose.material.Surface
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.businesscard.ui.theme.BusinessCardTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            BusinessCardTheme {
                // A surface container using the 'background' color from the theme
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = Color.DarkGray
                ) {
                    BusinessCard(
                        "Jennifer Doe",
                        "Android Developer Extraordinaire",
                        "+11 (123) 444 555 666",
                        "@AndroidDev",
                        "joe.doe@android.com"
                    )
                }
            }
        }
    }
}

@Composable
fun BusinessCard(
    name: String,
    description: String,
    phone: String,
    social: String,
    email: String
) {
    Column(
        modifier = Modifier
            .fillMaxWidth()
            .fillMaxHeight()
    ) {
        Column(
            modifier = Modifier
                .weight(0.8f)
                .fillMaxWidth()
                .fillMaxHeight(),
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center
        ) {
            val androidIcon = painterResource(id = R.drawable.android_logo)
            Image(
                painter = androidIcon,
                contentDescription = null,
                modifier = Modifier.width(100.dp)
            )
            Text(
                text = name,
                fontSize = 40.sp,
                color = Color.White,
                modifier = Modifier.padding(top = 5.dp, bottom = 5.dp)
            )
            Text(
                text = description,
                fontSize = 15.sp,
                color = Color.White
            )
        }
        Column(
            modifier = Modifier
                .weight(0.2f)
                .fillMaxWidth()
                .fillMaxHeight()
        ) {
            val horizontalLineModifier = Modifier.fillMaxWidth().height(1.dp)
            val horizontalLineColor = Color.LightGray
            Surface(color = horizontalLineColor, modifier = horizontalLineModifier) {}
            DescriptionRow(iconId = R.drawable.android_logo, description = phone)
            Surface(color = horizontalLineColor, modifier = horizontalLineModifier) {}
            DescriptionRow(iconId = R.drawable.android_logo, description = social)
            Surface(color = horizontalLineColor, modifier = horizontalLineModifier) {}
            DescriptionRow(iconId = R.drawable.android_logo, description = email)
        }
    }
}

@Composable
fun DescriptionRow(
    iconId: Int,
    description: String
)
{
    Row(
        horizontalArrangement = Arrangement.Center,
        verticalAlignment = Alignment.CenterVertically
    ) {
        val icon = painterResource(id = iconId)
        Image(
            painter = icon,
            contentDescription = null,
            modifier = Modifier
                .width(90.dp)
                .height(30.dp)
                .padding(start = 40.dp, end = 20.dp)
        )
        Text(
            text = description,
            color = Color.White
        )
    }
}

@Preview(showBackground = true)
@Composable
fun DefaultPreview() {
    BusinessCardTheme {
        Surface(
            modifier = Modifier.fillMaxSize(),
            color = Color.DarkGray
        ) {
            BusinessCard(
                "Jennifer Doe",
                "Android Developer Extraordinaire",
                "+11 (123) 444 555 666",
                "@AndroidDev",
                "joe.doe@android.com"
            )
        }
    }
}