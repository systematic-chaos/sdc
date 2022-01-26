import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator, PercentFormatter
from os import path, mkdir

data_path = path.abspath(path.join('data', 'lab1_data.xlsx'))

figures_path = 'images'
if not path.exists(figures_path):
    mkdir(figures_path, mode=0o740)



def load_data(data_sheet_path, data_sheet_index=0):
    return pd.read_excel(data_sheet_path, sheet_name=data_sheet_index, index_col=0, decimal=',')

def save_figure(fig_name, ext='jpg'):
    plt.savefig(path.join(figures_path, fig_name + '.' + ext))

def individual_measures(voltage_df, save_plot=True):
    # Initialize plot axis, enabling subplotting for individual line plots
    axis = voltage_df.plot(title='Individual measures from power sources',\
        subplots=True, figsize=(8, 9), ylim=[4.2, 5.8], legend=False)
    
    # Customize axis for each subplot
    for ax in axis:
        ax.axhline(y=5, color='k', linewidth=2)
        ax.get_xaxis().set_visible(False)
        ax.get_yaxis().set_major_locator(MultipleLocator(0.4))
        ax.get_yaxis().set_minor_locator(MultipleLocator(0.2))
        ax.grid(visible=True, axis='y', which='minor', color='lightgray')
        ax.spines['right'].set(visible=False)
        ax.spines['top'].set(color='lightgray')
        ax.spines['bottom'].set(color='lightgray')
    
    # Crop and adjust the figure canvas
    fig = axis[0].get_figure()
    fig.tight_layout()
    fig.subplots_adjust(top=0.95)
    
    # Store the plot's figure and return the resulting dataset
    if save_plot:
        save_figure('fig1')
    return voltage_df

def deviation_ref_5v(voltage_df, save_plot=True):
    # Compute absolute deviation from the reference value of 5V for every data sample
    abs_deviation_df = (voltage_df - 5).abs()

    # Initialize plot and customize its axis
    axis = abs_deviation_df.plot(title='Deviation from reference value (5V)',\
        alpha=0.6, ylim=[0, 0.8])
    axis.get_xaxis().set_visible(False)
    axis.set_yticks(np.arange(0, 1, 0.2))
    axis.grid(visible=True, axis='y', color='lightgray')
    axis.spines['top'].set(visible=False)
    axis.spines['right'].set(color='lightgray')

    # Store the plot's figure and return the resulting dataset
    if save_plot:
        save_figure('fig2')
    return abs_deviation_df

def stacked_deviation_ref_5v(abs_deviation_df, save_plot=True):
    # Compute absolute deviation from the reference value of 5V for every sample,
    # stacked in percentual terms for each data row.
    abs_dev_100_df = abs_deviation_df.div(abs_deviation_df.sum(axis=1), axis=0) * 100

    # Initialize stacked area plot and customize its axis
    axis = abs_dev_100_df.plot.area(title='Stacked deviation from reference value (5V)',\
        ylim=[0, 100], alpha=0.5)
    axis.get_xaxis().set_visible(False)
    axis.get_yaxis().set_major_formatter(PercentFormatter())

    # Store the plot's figure and return the resulting dataset
    if save_plot:
        save_figure('fig3')
    return abs_dev_100_df



# Load dataset, generate three plots, transforming the data as needed
# and saving the chart figures generated, and display them.
if __name__ == '__main__':
    df = load_data(data_path)
    df = individual_measures(df)
    df = deviation_ref_5v(df)
    stacked_deviation_ref_5v(df)
    plt.show()
