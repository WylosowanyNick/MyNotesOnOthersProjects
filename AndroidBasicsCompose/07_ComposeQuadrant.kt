package com.example.composequadrant

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.*
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Surface
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.example.composequadrant.ui.theme.ComposeQuadrantTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            ComposeQuadrantTheme {
                // A surface container using the 'background' color from the theme
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colors.background
                ) {
                    ComposeQuadrantApp()
                }
            }
        }
    }
}

@Composable
fun ComposeQuadrantApp()
{
    Column(
        modifier = Modifier
            .fillMaxWidth()
            .fillMaxHeight()
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .fillMaxHeight()
                .weight(1.0f)
        ) {
            Quadrant(
                name = stringResource(R.string.title_text),
                description = stringResource(R.string.desc_text),
                modifier = Modifier.weight(1.0f),
                color = Color.Green
            )
            Quadrant(
                name = stringResource(R.string.title_image),
                description = stringResource(R.string.desc_image),
                modifier = Modifier.weight(1.0f),
                color = Color.Yellow
            )
        }
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .fillMaxHeight()
                .weight(1.0f)
        ) {
            Quadrant(
                name = stringResource(R.string.title_row),
                description = stringResource(R.string.desc_row),
                modifier = Modifier.weight(1.0f),
                color = Color.Cyan
            )
            Quadrant(
                name = stringResource(R.string.title_column),
                description = stringResource(R.string.desc_column),
                modifier = Modifier.weight(1.0f),
                color = Color.LightGray
            )
        }
    }
}

@Composable
private fun Quadrant(
    name: String,
    description: String,
    modifier: Modifier,
    color: Color = Color.Transparent
) {
    Surface(color = color, modifier = modifier) {
        Column(
            modifier = Modifier
                .fillMaxHeight()
                .padding(16.dp),
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center
        ) {
            Text(
                text = name,
                fontWeight = FontWeight.Bold,
                modifier = Modifier.padding(bottom = 16.dp)
            )
            Text(text = description, textAlign = TextAlign.Justify)
        }
    }
}

@Preview(showBackground = true)
@Composable
fun DefaultPreview() {
    ComposeQuadrantTheme {
        ComposeQuadrantApp()
    }
}